/*
 * @file variables.h
 */

#ifndef VARIABLES_H
#define VARIABLES_H

struct DccCommand {
  uint16_t addr;
  uint8_t direction;
};

DccCommand commandQueue[QUEUE_SIZE];

volatile uint8_t queueHead = 0;
volatile uint8_t queueTail = 0;

NmraDcc Dcc;

const uint8_t gammaTable[] = {
  255,244,234,225,216,207,199,191,183,175,168,161,153,147,140,133,127,121,115,110,104,99,94,89,84,80,75,71,67,64,60,56,53,50,47,44,41,38,36,33,31,29,27,25,23,21,20,18,17,15,14,13,12,11,10,9,8,7,6,6,5,5,4,4,3,3,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0
};

enum LightState { UP, HOLD, DOWN, OFF };
struct CrossingLight { int pin; int brightness; LightState state; unsigned long timer; };

CrossingLight light1 = {LED1, 0, UP, 0};
CrossingLight light2 = {LED2, 0, OFF, 0};

uint16_t baseAddress;
bool isLearning = false;                  // in learning mode

bool crossingActive = false;
bool lightsShouldRun = false;
bool lightsActive = false;
bool bellActive = false;
bool servo1Active = false;
bool servo2Active = false;

bool signalInverted = false;

int gate1Up, gate1Down, gate1Speed;       // servo1 CVs
int gate2Up, gate2Down, gate2Speed;       // servo2 CVs
unsigned long closeWarningTime;
unsigned long openExtensionTime;

unsigned long blinkTime;                  // alternate flash time from CV
int led1Type;                             // led 1 type
int led2Type;                             // led 2 type


Servo servo1, servo2;

int pos1, pos2;                           // current servo position
unsigned long lastUpdate = 0;

unsigned long warningStartTimer = 0;
unsigned long s1Timer = 0, s2Timer = 0;
unsigned long s1Delay = 0, s2Delay = 0;
bool s1Waiting = false, s2Waiting = false;

uint8_t bellType = 0;

unsigned long lastStrikeTime = 0;

uint16_t frequency = 0;
uint32_t interval = 0;
uint32_t strikeDuration = 0;
bool isRinging = false;


// for bellType = 1   // bell from ise

volatile uint32_t audio_index = 0;
volatile bool is_playing = false;
HardwareTimer *SampleTimer = NULL;



#endif
