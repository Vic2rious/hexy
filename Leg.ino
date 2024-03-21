#include <Servo.h>
#include <Wire.h>
//#include <PS4Controller.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>

//These consts are from the library. DO NOT TOUCH
#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//JOINT CHANNEL DEFINITIONS
#define coxa1Channel 0
#define femur1Channel 1
#define tibia1Channel 2

#define coxaOffset  20 //It's a bit offcenter so this is my fix
#define femurOffset 35

bool led = true;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN , OUTPUT);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  
  //Setting default angles 
  setAngle(coxa1Channel,90-coxaOffset);
  delay(300);
  setAngle(femur1Channel,90-femurOffset);
  delay(300);
 setAngle(tibia1Channel,130);
}

void setAngle(uint8_t channel, int angle){

if (angle < 0) 
    angle = 0;
   else if (angle > 180) 
    angle = 180;
  

  uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}


void loop() {

  if(led){
  //To check things are running and there are no electrical problems! 
  digitalWrite(LED_BUILTIN , HIGH);
  delay(500);                     
  digitalWrite(LED_BUILTIN , LOW);
  delay(500);    
  }                   
}