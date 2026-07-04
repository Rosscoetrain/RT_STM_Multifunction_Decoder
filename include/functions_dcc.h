/*
 * @file functions_dcc.h
 */

#ifndef FUNCTIONS_DCC_H
#define FUNCTIONS_DCC_H


#include <EEPROM.h>

// DCC functions

void notifyCVAck()
 {
  
 }




void notifyDccMsg(DCC_MSG * Msg)
 {
  if (Msg->Data[0] == 0xFF || Msg->Data[0] == 0x7F) return;

  blinkAck(3);

/*
// Service Mode (Programming Track) - 3 Byte Packet

  if (Msg->Size == 3 && (Msg->Data[0] & 0xF0) == 0x70)
   {
    uint16_t cvNum = Msg->Data[1] + 1;
    uint8_t cvVal = Msg->Data[2];

    Dcc.setCV(cvNum, cvVal);
//    blinkAck();
    if (cvNum == 8 && cvVal == 8)
     {
      resetDefaults();
     }
    return;
   }

// POM - 4 byte packet

  if (Msg->Size >= 4 && (Msg->Data[1] & 0xF0) == 0xE0)
   {
    uint16_t packetAddr = Msg->Data[0];
    if (packetAddr == baseAddress)
     {
      uint16_t cv = ((uint16_t)(Msg->Data[1] & 0x03) << 8) + Msg->Data[2] + 1;
      uint8_t newVal = Msg->Data[3];
      if (cv == 8 && newVal == 8)
       {
        resetDefaults();
        return;
       }
      bool changed = false;
      if (cv == CV_GATE1_UP) { gate1Up = newVal; changed = true; }
      else if (cv == CV_GATE1_DOWN) { gate1Down = newVal; changed = true; }
      else if (cv == CV_GATE1_SPEED) { gate1Speed = (newVal < 1) ? 1 : newVal; changed = true; }
      else if (cv == CV_GATE2_UP) { gate2Up = newVal; changed = true; }
      else if (cv == CV_GATE2_DOWN) { gate2Down = newVal; changed = true; }
      else if (cv == CV_GATE2_SPEED) { gate2Speed = (newVal < 1) ? 1 : newVal; changed = true; }
      else if (cv == CV_CLOSE_WARNING) { closeWarningTime = (unsigned long)newVal * 100; changed = true; }
      else if (cv == CV_OPEN_EXTENSION) { openExtensionTime = (unsigned long)newVal * 100; changed = true; }
      else if (cv == CV_SIGNAL_INV) { signalInverted = (newVal > 0); changed = true; }
      else if (cv == CV_BELL_TYPE) { bellType=newVal; changed = true; }
      else if (cv == CV_BELL_FREQ_HZ) { frequency=newVal * 10; changed = true; }
      else if (cv == CV_BELL_INTERVAL) { interval=newVal * 10; changed = true; }
      else if (cv == CV_BELL_STRIKETIME) { strikeDuration=newVal * 10; changed = true; }
      else if (cv == CV_BLINK_TIME) { blinkTime=newVal * 10; changed = true; }

      if (changed) Dcc.setCV(cv, newVal);
     }
   }
*/
 }




void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
 {
  if (isLearning)
   {
    baseAddress = Addr;
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, (Addr & 0x3F));
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, ((Addr >> 6) & 0x07));
    isLearning = false;
    blinkAck(2);
//    for(int i=0; i<3; i++) { digitalWrite(LED1, LOW); delay(200); digitalWrite(LED1, HIGH); delay(200); }
   }
  else
   {
    uint8_t nextHead = (queueHead + 1) % QUEUE_SIZE;
    if (nextHead != queueTail)
     {
      commandQueue[queueHead].addr = Addr;
      commandQueue[queueHead].direction = Direction;
      queueHead = nextHead;
     }
   }
 }


uint8_t notifyCVWrite (uint16_t CV, uint8_t Value)
 {

#ifdef DEBUG_MSG
  MYSERIAL.print("notifyCVWrite cv : ");
  MYSERIAL.print(CV);
  MYSERIAL.print(" value : ");
  MYSERIAL.println(Value);
#endif


  if (CV <= CV_BELL_STRIKETIME)
   {
    if ((CV == 8) && (Value == 8))  // factory reset return before writing to CV
     {
      resetDefaults();
      return Value;
     }
    if (CV == 8)                    // ignore any attempts to write to CV8
     {
      return Value;
     }
    if ((CV == 1) || (CV == 9))     // ignore writes to CV1 and CV9 causes problems with actual address
     {
      return Value;
     }
    if ((CV == CV_SIGNAL_INV) && (Value > 1))     // only accept 0 or 1 for CV_SIGNAL_INV
     {
      return Value;
     }
    if ((CV == CV_BELL_TYPE) && (Value > 3))     // only accept 0 - 3 for CV_BELL_TYPE
     {
      return Value;
     }
    EEPROM.update(CV, Value);
   }

  return Value;
 }


#endif
