/*
 * @file functions.h
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#ifdef BELL_3
#endif

void writeSignal(bool active)
 {
  bool level = active;
  if (signalInverted) level = !active;
  digitalWrite(BELL_OUT, level ? HIGH : LOW);
 }

void blinkAck(int b)
 {
  for (int i=0;i<b;i++)
   {
    digitalWrite(STATUS_LED, HIGH); delay(100); digitalWrite(STATUS_LED, LOW); delay(100);
   }
 }


void resetDefaults()
 {
  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, (DEFAULT_ADDR & 0x3F));
  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, ((DEFAULT_ADDR >> 6) & 0x07));
  Dcc.setCV(CV_GATE1_UP, DEFAULT_GATE_UP);
  Dcc.setCV(CV_GATE1_DOWN, DEFAULT_GATE_DOWN);
  Dcc.setCV(CV_GATE1_SPEED, DEFAULT_SPEED);
  Dcc.setCV(CV_GATE2_UP, DEFAULT_GATE_UP);
  Dcc.setCV(CV_GATE2_DOWN, DEFAULT_GATE_DOWN);
  Dcc.setCV(CV_GATE2_SPEED, DEFAULT_SPEED);
  Dcc.setCV(CV_CLOSE_WARNING, DEFAULT_TIME_VAL);
  Dcc.setCV(CV_OPEN_EXTENSION, DEFAULT_TIME_VAL);
  Dcc.setCV(CV_SIGNAL_INV, 0);

  Dcc.setCV(CV_BLINK_TIME, DEFAULT_BLINKTIME / 10);   // multipy this by 10 to get time in ms
  Dcc.setCV(CV_LED_1_TYPE, DEFAULT_LED_1);
  Dcc.setCV(CV_LED_2_TYPE, DEFAULT_LED_2);

  Dcc.setCV(CV_BELL_TYPE, BELL_TYPE_DEFAULT);
  Dcc.setCV(CV_BELL_FREQ_HZ, BELL_FREQ_HZ / 10);     // multiply this by 10 to get actual frequency
  Dcc.setCV(CV_BELL_INTERVAL, BELL_INTERVAL / 10);     // multiply this by 10 to get actual interval
  Dcc.setCV(CV_BELL_STRIKETIME, BELL_STRIKETIME / 10);  // multiply this by 10 to get actual strietime

  gate1Up = gate2Up = DEFAULT_GATE_UP;
  gate1Down = gate2Down = DEFAULT_GATE_DOWN;
  gate1Speed = gate2Speed = DEFAULT_SPEED;
  closeWarningTime = openExtensionTime = DEFAULT_TIME_VAL * 100;
  signalInverted = false;
  blinkTime = DEFAULT_BLINKTIME;
  led1Type = DEFAULT_LED_1;
  led2Type = DEFAULT_LED_2;

  baseAddress = DEFAULT_ADDR;

  bellType = BELL_TYPE_DEFAULT;
  frequency = BELL_FREQ_HZ;
  interval = BELL_INTERVAL;
  strikeDuration = BELL_STRIKETIME;

  blinkAck(5);
 }


void updateGate() {
  if (isLearning) return;
//  int target1 = crossingActive ? gate1Down : gate1Up;
//  int target2 = crossingActive ? gate2Down : gate2Up;

  bool s1 = (crossingActive || servo1Active);
  bool s2 = (crossingActive || servo2Active);

  int target1 = s1 ? gate1Down : gate1Up;
  int target2 = s2 ? gate2Down : gate2Up;

  if (crossingActive) {
    if (warningStartTimer == 0) warningStartTimer = millis();
    if (millis() - warningStartTimer < closeWarningTime) {
      target1 = pos1; target2 = pos2;
    }
  } else { warningStartTimer = 0; }


//  if ((crossingActive && pos1 == gate1Up && (millis() - warningStartTimer >= closeWarningTime)) ||
  if ((s1 && pos1 == gate1Up && (millis() - warningStartTimer >= closeWarningTime)) ||
      (!crossingActive && pos1 == gate1Down)) {
    if (!s1Waiting) { s1Timer = millis(); s1Delay = random(500, 2500); s1Waiting = true; }
    if (millis() - s1Timer < s1Delay) target1 = pos1;
  } else { s1Waiting = false; }


//  if ((crossingActive && pos2 == gate2Up && (millis() - warningStartTimer >= closeWarningTime)) ||
  if ((s2 && pos2 == gate2Up && (millis() - warningStartTimer >= closeWarningTime)) ||
      (!crossingActive && pos2 == gate2Down)) {
    if (!s2Waiting) { s2Timer = millis(); s2Delay = random(500, 2500); s2Waiting = true; }
    if (millis() - s2Timer < s2Delay) target2 = pos2;
  } else { s2Waiting = false; }


  if (pos1 != target1) {
    if (!servo1.attached()) servo1.attach(SERVO_PIN);
    pos1 += (pos1 < target1) ? gate1Speed : -gate1Speed;
    if (abs(pos1 - target1) < gate1Speed) pos1 = target1;
    servo1.writeMicroseconds(map(pos1, 0, 255, 1000, 2000));
  } else if (servo1.attached()) servo1.detach();


  if (pos2 != target2) {
    if (!servo2.attached()) servo2.attach(SERVO2_PIN);
    pos2 += (pos2 < target2) ? gate2Speed : -gate2Speed;
    if (abs(pos2 - target2) < gate2Speed) pos2 = target2;
    servo2.writeMicroseconds(map(pos2, 0, 255, 1000, 2000));
  } else if (servo2.attached()) servo2.detach();
}


void updateBell()
 {
  unsigned long currentTime = millis();

  if (!bellType) return;

//  if (lightsShouldRun)
  if (bellActive || crossingActive)
   {
    switch (bellType)
     {
      case 1:                     // low output on BELL_OUT for iowa scalled engineering type sound module
        digitalWrite(BELL_OUT, signalInverted ? HIGH : LOW);
        break;
      case 2:                     // onboard ampilfied speaker
        if (!isRinging && (currentTime - lastStrikeTime >= interval))
         {
// void tone(uint8_t pin, unsigned int frequency, unsigned long duration = 0);
          tone(BELL_OUT, frequency);
          isRinging = true;
          lastStrikeTime = currentTime;
         }

        if (isRinging && (currentTime - lastStrikeTime >= strikeDuration))
         {
          noTone(BELL_OUT);
          isRinging = false;
         }
        break;
      case 3:                     // onboard speaker with wav file
        break;
      default:
        break;
     }
   }
  else
   {
    switch (bellType)
     {
      case 1:                     // high output on BELL_OUT for iowa scalled engineering type sound module
        digitalWrite(BELL_OUT, signalInverted ? LOW : HIGH);
        break;
      case 2:                     // onboard ampilfied speaker
        noTone(BELL_OUT);
        isRinging = false;
        analogWrite(BELL_OUT, 0);
        break;
      case 3:                     // onboard speaker with wav file
        break;

      default:
        break;
     }
   }
 }



void updateLightLogic()
 {
  static unsigned long exitTimer = 0;
//  if (crossingActive || pos1 != gate1Up || pos2 != gate2Up) {
//  if (lightsActive || crossingActive || (!servo1Active && pos1 != gate1Up) || (!servo2Active && pos2 != gate2Up))
  if (lightsActive || crossingActive)
   {
    lightsShouldRun = true;
    exitTimer = 0;
   }
  else
   {
    if (exitTimer == 0)
     {
      exitTimer = millis();
     }
    if (millis() - exitTimer > openExtensionTime)
     {
      lightsShouldRun = false;
     }
   }
 }

void updateLight(CrossingLight &l, CrossingLight &partner) {
  if (isLearning) return;
  switch (l.state) {
    case UP:
      l.brightness += 15;
      if (l.brightness >= 255) { l.brightness = 255; l.state = HOLD; l.timer = millis(); }
      break;
//    case HOLD: if (millis() - l.timer >= 500) l.state = DOWN; break;
    case HOLD: if (millis() - l.timer >= blinkTime) l.state = DOWN; break;
    case DOWN:
      l.brightness -= 15;
      if (l.brightness <= 0) { l.brightness = 0; l.state = OFF; }
      break;
    case OFF: if (lightsShouldRun && partner.state == OFF) l.state = UP; break;
  }
  uint8_t index = map(l.brightness, 0, 255, 0, 84);
  analogWrite(l.pin, gammaTable[index]);
}

void processQueue()
 {
  if (queueHead == queueTail) return;

  uint16_t Addr = commandQueue[queueTail].addr;
  uint8_t Direction = commandQueue[queueTail].direction;
  queueTail = (queueTail + 1) % QUEUE_SIZE;

  if ((Addr >= baseAddress) && (Addr <= baseAddress + 5))
   {
    if (Addr == baseAddress)                    // all
     {
      crossingActive = (Direction == 1);
     }

    if (Addr == baseAddress + 1)                // leds
     {
      lightsActive = (Direction == 1);
     }

    if (Addr == baseAddress + 2)                // bell
     {
      bellActive = (Direction == 1);
     }

    if (Addr == baseAddress + 3)                // servo 1
     {
      servo1Active = (Direction == 1);
     }

    if (Addr == baseAddress + 4)                // servo 2
     {
      servo2Active = (Direction == 1);
     }

    if (Addr == baseAddress + 5)                // servo both
     {
      servo1Active = (Direction == 1);
      servo2Active = (Direction == 1);
     }
   }
 }



#ifdef BELL_3
// Interrupt Service Routine (ISR) triggered exactly at 8000 Hz for bellType = 1
void inline audioSampleISR(void)
 {
//    if (is_playing) {
  if ((lightsShouldRun) && (is_playing))
   {
// Feed current audio byte into the PWM controller
    analogWrite(BELL_OUT, AUDIO_DATA[audio_index]);
    audio_index++;

// Stop playback when the index reaches the array boundary
    if (audio_index >= AUDIO_LENGTH)
     {
      is_playing = false;

// FIXED: Using official stm32duino .pause() method instead of .stop()
//      SampleTimer->pause();   // move this to the updateBell function
      audio_index = 0;
      analogWrite(BELL_OUT, 0); // Ground the pin to remove DC hiss
     }
   }
 }
#endif


extern "C" int _write(int file, char *ptr, int len) { return len; }




#endif
