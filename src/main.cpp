/*
 *
 * (C) 2026 Ross Scanlon
 *
 * RosscoeTrain
 *
 * iam@rosscoe.com
 *
 * stm32 DCC decoder for a multiple functions.
 * 
 * Level crossing
 * Dual servos
 * Flashing leds
 * Bell
 *
 */


#include <Arduino.h>
#include <NmraDcc.h>
#include <Servo.h>

#include "defines.h"
#include "variables.h"
#include "functions.h"
#include "functions_dcc.h"

#ifdef BELL_3
#endif


void setup()
 {

  pinMode(SERVO_POWER_PIN, OUTPUT);
  digitalWrite(SERVO_POWER_PIN, LOW);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  analogWrite(LED1, 0);
  analogWrite(LED2, 0);

  pinMode(STATUS_LED, OUTPUT);
  pinMode(BELL_OUT, OUTPUT);

  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BELL_OUT, signalInverted);

  pinMode(LEARN_PIN, INPUT);
  delay(20);
  // Check Learning Mode (Active High)
  if (digitalRead(LEARN_PIN) == HIGH)
   {
    isLearning = true;
    digitalWrite(STATUS_LED, HIGH);
   }

  baseAddress = Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) | (Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) << 6);

  blinkAck(baseAddress);

  // Correction: Use digitalPinToInterrupt for STM32 compatibility
  Dcc.pin(digitalPinToInterrupt(DCC_PIN), DCC_PIN, false);
  Dcc.init(MAN_ID_DIY, 100, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0);

  gate1Up = Dcc.getCV(CV_GATE1_UP);
  gate1Down = Dcc.getCV(CV_GATE1_DOWN);
  gate1Speed = Dcc.getCV(CV_GATE1_SPEED);
  gate2Up = Dcc.getCV(CV_GATE2_UP);
  gate2Down = Dcc.getCV(CV_GATE2_DOWN);
  gate2Speed = Dcc.getCV(CV_GATE2_SPEED);
  closeWarningTime = (unsigned long)Dcc.getCV(CV_CLOSE_WARNING) * 100;
  openExtensionTime = (unsigned long)Dcc.getCV(CV_OPEN_EXTENSION) * 100;
  signalInverted = (Dcc.getCV(CV_SIGNAL_INV) > 0);
  blinkTime = Dcc.getCV(CV_BLINK_TIME) * 10;

  led1Type = Dcc.getCV(CV_LED_1_TYPE);
  led2Type = Dcc.getCV(CV_LED_2_TYPE);


  bellType = Dcc.getCV(CV_BELL_TYPE);
  frequency = Dcc.getCV(CV_BELL_FREQ_HZ) * 10;
  interval = Dcc.getCV(CV_BELL_INTERVAL) * 10;
  strikeDuration = Dcc.getCV(CV_BELL_STRIKETIME) * 10;


  pos1 = gate1Up;
  pos2 = gate2Up;

  servo1.detach();
  servo2.detach();


#ifdef BELL_3
  if (bellType == 3)
   {
// Configure carrier parameters for 8-bit resolution at 187.5 kHz
    analogWriteResolution(8); 
    analogWriteFrequency(187500);
    analogWrite(BELL_OUT, 128); // Set silent baseline voltage bias

    // Force-isolate TIM14 register base for our background sample paces
    TIM_TypeDef *Instance = TIM14; 
    
    SampleTimer = new HardwareTimer(Instance);

    // Attach our target 8 kHz sample conversion processing loop
//    SampleTimer->setOverflow(8000, HERTZ_FORMAT); 
    SampleTimer->setOverflow(16000, HERTZ_FORMAT); 
    SampleTimer->attachInterrupt(audioSampleISR);
    
    // Arm variables and kick-off playback execution
    audio_index = 0;
    is_playing = false;
//    SampleTimer->resume();  // move this to the updateBell function
   }
#endif

  delay(20);
  digitalWrite(SERVO_POWER_PIN, HIGH);

 }

void loop()
 {
  Dcc.process();
  processQueue();

  if (millis() - lastUpdate >= 20)
   {
    lastUpdate = millis();
    updateGate();
    updateLightLogic();
    updateLight(light1, light2);
    updateLight(light2, light1);
    updateBell();
   }

 }

