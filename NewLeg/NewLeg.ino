/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOMID 375 // hopefully 90 degrees
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

  
#define coxaOffset  0 //It's a bit offcenter so this is my fix
#define femurOffset -40
#define tibiaOffset + 110


// our servo # counter
uint8_t servonum = 0;

const int legCount = 6;
const int jointCount = 3;

int direction;
int yPos;
bool isZero = true;


class Joint {
public:
    uint8_t channel;
    int offset;
    bool first;

    Joint(uint8_t channel, int offset, bool first) {
        this->channel = channel;
        this->offset = offset;
        this->first = first;
    }
void setAngle(int angle) {
  
        constrain(angle, 0, 180);  
        
        uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX); 
        if(first) {
          pwm1.setPWM(channel, 0, angle);
        } else {
          pwm2.setPWM(channel, 0, angle);
        }
    }

void setPulse(int pulse) {
    if(first) {
      pwm1.setPWM(channel, 0, pulse);
    } else {
      pwm2.setPWM(channel, 0, pulse);
    }
}
    

};

const int coxaLength = 51;  //leg part lengths in mm
const int femurLength = 65;
const int tibiaLength = 121;

Joint L1_coxa(0, coxaOffset - 50, true);
Joint L1_femur(1, femurOffset - 70, true);
Joint L1_tibia(2, tibiaOffset - 70, true);

Joint L2_coxa(3, coxaOffset + 80, true);
Joint L2_femur(4, femurOffset + 90, true);
Joint L2_tibia(5, tibiaOffset - 120, true);

Joint L3_coxa(6, coxaOffset - 90, true);
Joint L3_femur(7, femurOffset + 20, true);
Joint L3_tibia(8, tibiaOffset + 20, true);

Joint L4_coxa(0, coxaOffset, false);
Joint L4_femur(1, femurOffset, false);
Joint L4_tibia(2, tibiaOffset, false);

Joint L5_coxa(3, coxaOffset + 20, false);
Joint L5_femur(4, femurOffset + 20, false);
Joint L5_tibia(5, tibiaOffset, false);

Joint L6_coxa(6, coxaOffset, false);
Joint L6_femur(7, femurOffset + 80, false);
Joint L6_tibia(8, tibiaOffset, false);

const Joint coxas[6] = {L1_coxa,L2_coxa,L3_coxa,L4_coxa,L5_coxa,L6_coxa};
const Joint femurs[6] = {L1_femur,L2_femur,L3_femur,L4_femur,L5_femur,L6_femur};
const Joint tibias[6] = {L1_tibia,L2_tibia,L3_tibia,L4_tibia,L5_tibia,L6_tibia};

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm1.begin();
  pwm2.begin();
  Serial.println("pwm started");
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm1.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  
  pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(200);
  
  
  //L2_coxa.setPulse(275);
  resetLegPositions();

  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

void resetLegPositions(){

   //Setting default angles 
  for(int i = 0; i<legCount;i++){
    //delay(100);
    coxas[i].setPulse(375-coxas[i].offset);
    //delay(100);
    femurs[i].setPulse(375-femurs[i].offset);
    //delay(100);
    tibias[i].setPulse(375-tibias[i].offset);

    
  }
}

void forwardCycle(int speed, int height, bool forward) {

  getUp(forward, height);

  delay(1000);

  for(int i = 0; i<legCount;i+=2){
    //delay(100);
    direction = i > 2 ? -1 : 1;
    coxas[i].setPulse(375-(speed*direction)-coxas[i].offset);
    direction = i+1 > 2 ? -1 : 1;
    coxas[i+1].setPulse(375+(speed*direction)-coxas[i+1].offset);
    
    //delay(100);
    
    //delay(100);
    //tibias[i].setPulse(375-tibias[i].offset);  
  }

  delay(1000);

  forward ^= true;
  getUp(forward, height);
  
  delay(1000);

  for(int i = 0; i<legCount;i+=2){
    //delay(100);
    direction = i > 2 ? -1 : 1;
    coxas[i].setPulse(375+(speed*direction)-coxas[i].offset);
    direction = i+1 > 2 ? -1 : 1;
    coxas[i+1].setPulse(375-(speed*direction)-coxas[i+1].offset);
    
    //delay(100);
    //femurs[i].setPulse(375-femurs[i].offset);
    //delay(100);
    //tibias[i].setPulse(375-tibias[i].offset);  
  }

  delay(1000);

}

void getUp (bool forward, int height) { // like a sex machine yeah

  if (forward) {
      for(int i = 0; i<legCount;i+=2){
      
      femurs[i].setPulse(375-height-femurs[i].offset);
      femurs[i+1].setPulse(375-femurs[i+1].offset);
    }
  } else {
      for(int i = 0; i<legCount;i+=2){
      
      femurs[i].setPulse(375-femurs[i].offset);
      femurs[i+1].setPulse(375-height-femurs[i+1].offset);
    }
  }
} 

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!

int setSpeed() {
  
  
} 

void loop() {
  // Drive each servo one at a time using setPWM()
  if(Serial.available() > 1) {

    String data = Serial.readStringUntil('\n');

    yPos = data.toInt();

    int speed = yPos < 0 ? yPos*-1 : yPos;


    if(yPos != 0) {
      speed = constrain(speed, 10, 40);
      forwardCycle(speed, 60, yPos > 0 ? true : false);
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    
  }
  Serial.println("blase");
  


//  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
//    pwm.setPWM(servonum, 0, pulselen);
//  }

  //delay(500);
  
  //delay(500);
  
//  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
//    pwm.setPWM(servonum, 0, pulselen);
//  }

  //delay(500);

  // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
  // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior. 
//  for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) {
//    pwm.writeMicroseconds(servonum, microsec);
//  }
//
//  delay(500);
//  for (uint16_t microsec = USMAX; microsec > USMIN; microsec--) {
//    pwm.writeMicroseconds(servonum, microsec);
//  }
//
//  delay(500);


}