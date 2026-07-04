/*
 * @file defines.h
 */

#ifndef DEFINES_H
#define DEFINES_H

  #define LEARN_PIN  PA_6
  #define DCC_PIN    PA_8

  #define SERVO_PIN  PA_5
  #define SERVO2_PIN PA_3
  #define SERVO_POWER_PIN PA_4

  #define LED2       PA_0
  #define LED1       PA_1
  #define BELL_OUT   PA_2
//  #define STATUS_LED PB7
  #define STATUS_LED PA_5


// uncomment the following to use bellType = 3 for included wav sound

//#define BELL_3

#define DEFAULT_ADDR      1
#define DEFAULT_GATE_UP   180
#define DEFAULT_GATE_DOWN 50
#define DEFAULT_SPEED     1
#define DEFAULT_TIME_VAL  30

#define CV_GATE1_UP       33
#define CV_GATE1_DOWN     34
#define CV_GATE1_SPEED    35               // servo 1 speed of movement higher = faster
#define CV_GATE2_UP       36
#define CV_GATE2_DOWN     37
#define CV_GATE2_SPEED    38
#define CV_CLOSE_WARNING  39
#define CV_OPEN_EXTENSION 40
#define CV_SIGNAL_INV     41


#define CV_BLINK_TIME     42               // alternate flash rate
#define CV_LED_1_TYPE     43
#define CV_LED_2_TYPE     44

#define DEFAULT_BLINKTIME 500
#define DEFAULT_LED_1     1
#define DEFAULT_LED_2     1


#define CV_BELL_TYPE      45
#define CV_BELL_FREQ_HZ   46
#define CV_BELL_INTERVAL  47
#define CV_BELL_STRIKETIME 48

#define BELL_TYPE_DEFAULT 1
#define BELL_FREQ_HZ    750
#define BELL_INTERVAL   1000
#define BELL_STRIKETIME 120



#define QUEUE_SIZE 16


#endif
