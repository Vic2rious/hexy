#include <Servo.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>


//These consts are from the library. DO NOT TOUCH
#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

const int legCount = 6;
const int jointCount = 3;

bool ik = true;

double theta1,theta2,theta3;

class Joint {
public:
    uint8_t channel;
    int offset;
    Adafruit_PWMServoDriver pwm;

    Joint(uint8_t channel, int offset,Adafruit_PWMServoDriver pwm ) {
        this->channel = channel;
        this->offset = offset;
        this-> pwm = pwm;
    }
void setAngle(int angle) {
        constrain(angle, 0, 180);  
        uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX); 
        pwm.setPWM(channel, 0, pulse);
    }
    

};

  
#define coxaOffset  20 //It's a bit offcenter so this is my fix
#define femurOffset 35

const int coxaLength = 51;  //leg part lengths in mm
const int femurLength = 65;
const int tibiaLength = 121;

bool led = true;

Joint L1_coxa(0, coxaOffset,pwm1);
Joint L1_femur(1, femurOffset,pwm1);
Joint L1_tibia(2, 0,pwm1);

Joint L2_coxa(3, coxaOffset,pwm1);
Joint L2_femur(4, femurOffset,pwm1);
Joint L2_tibia(5, 0,pwm1);

Joint L3_coxa(6, coxaOffset,pwm1);
Joint L3_femur(7, femurOffset,pwm1);
Joint L3_tibia(8, 0,pwm1);

Joint L4_coxa(9, coxaOffset,pwm1);
Joint L4_femur(10, femurOffset,pwm1);
Joint L4_tibia(11, 0,pwm1);

Joint L5_coxa(0, coxaOffset,pwm2);
Joint L5_femur(1, femurOffset,pwm2);
Joint L5_tibia(2, 0,pwm2);

Joint L6_coxa(3, coxaOffset,pwm2);
Joint L6_femur(4, femurOffset,pwm2);
Joint L6_tibia(5, 0,pwm2);

const Joint coxas[6] = {L1_coxa,L2_coxa,L3_coxa,L4_coxa,L5_coxa,L6_coxa};
const Joint femurs[6] = {L1_femur,L2_femur,L3_femur,L4_femur,L5_femur,L6_femur};
const Joint tibias[6] = {L1_tibia,L2_tibia,L3_tibia,L4_tibia,L5_tibia,L6_tibia};
int xyValues [2];

void setup() {

  Serial.begin(9600);

  pwm1.begin();
  pwm2.begin();

  pwm1.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(SERVO_FREQ);

  pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(SERVO_FREQ);

 resetLegPositions();
}

void resetLegPositions(){

   //Setting default angles 
  for(int i = 0; i<legCount;i++){
    delay(100);
    coxas[i].setAngle(90-coxas[i].offset);
    delay(100);
    femurs[i].setAngle(90-femurs[i].offset);
    delay(100);
    tibias[i].setAngle(130);

    
  }
}

void loop() { 

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    char* ptr = strtok((char*)data.c_str(),",");
    int i = 0;

    while(ptr!=NULL && i<3){
      xyValues  [i++] = atoi(ptr);
      ptr = strtok(NULL,",");
    }


    if(ik)
    tripodGaitIK();
    else {
      tripodGaitBrute();
    }
  //forwardKinematics();
  delay(500);

}
}

void inverseKinematics(int x, int y, int z){
   theta1 = atan2(y,x); // Basic trig (tan = opp/adj)


  double xA = coxaLength*cos(theta1);
  double yA = coxaLength*sin(theta1);

  double xB = x-xA;
  double yB = y-yA;

  double P = xB /cos(theta1);

  double deltaZ = abs(z);
  double H = sqrt(pow(P,2)+pow(deltaZ,2));

  double phi3 = asin(deltaZ/H);

  double phi2Acos = (pow(tibiaLength, 2) + pow(H, 2) - pow(femurLength, 2)) / (2 * H * tibiaLength);
  double phi2 = acos(phi2Acos);
  double phi1Acos = (pow(femurLength, 2) + pow(H, 2) - pow(tibiaLength, 2)) / (2 * H * femurLength);
  double phi1 = acos(phi1Acos);


  if(z>0)
  theta2 = phi1+phi3;
  else theta2 = phi1-phi3;

  theta3 = phi1+phi2;

  if (isnan(phi3) || isnan(phi2) || isnan(phi1)) {
    Serial.println("NaN detected in intermediate values!"); // nqkude gurmi. Ponqkoga inputa za Acos e izvun rangea -1 i 1
    return;
  }

  theta1 = degrees(theta1);
  theta2 = degrees(theta2);
  theta3 = degrees(theta3);

  Serial.println(theta1);
  Serial.println(theta2);
  Serial.println(theta3);

}




void tripodGaitIK(){

  Joint phase1Coxas[3] = {L1_coxa,L3_coxa,L5_coxa};
  Joint phase1Femurs[3] = {L1_femur,L3_femur,L5_femur};
  Joint phase1Tibias[3] = {L1_tibia,L3_tibia,L5_tibia};

  Joint phase2Coxas[3] = {L2_coxa,L4_coxa,L6_coxa};
  Joint phase2Femurs[3] = {L2_femur,L4_femur,L6_femur};
  Joint phase2Tibias[3] = {L2_tibia,L4_tibia,L6_tibia};


 
      inverseKinematics(xyValues[0],xyValues[1],20);//The last value is Z which is equal to the amount of mm our legs will be lifted
      for(int i= 0; i<jointCount;i++){
        if(isOnTheLeft(phase1Coxas[i])
        phase1Coxas[i].setAngle(180-theta1);
        else phase1Coxas[i].SetAngle(theta1);

        phase1Femurs[i].setAngle(theta2);
        phase1Tibias[i].SetAngle(theta3);
      }
      delay(150);
      inverseKinematics(0,0,-20);
      for(int i= 0; i<jointCount;i++){
        if(isOnTheLeft(phase1Coxas[i])
        phase1Coxas[i].setAngle(180-theta1);
        else phase1Coxas[i].SetAngle(theta1);

        phase1Femurs[i].setAngle(theta2);
        phase1Tibias[i].SetAngle(theta3);
      }
inverseKinematics(xyValues[0],xyValues[1],20);//The last value is Z which is equal to the amount of mm our legs will be lifted
      for(int i= 0; i<jointCount;i++){
        if(isOnTheLeft(phase2Coxas[i])
        phase2Coxas[i].setAngle(180-theta1);
        else phase2Coxas[i].SetAngle(theta1);

        phase2Femurs[i].setAngle(theta2);
        phase2Tibias[i].SetAngle(theta3);
      }

      inverseKinematics(0,0,-20);//The last value is Z which is equal to the amount of mm our legs will be lifted
      for(int i= 0; i<jointCount;i++){
        if(isOnTheLeft(phase2Coxas[i])
        phase2Coxas[i].setAngle(180-theta1);
        else phase2Coxas[i].SetAngle(theta1);

        phase2Femurs[i].setAngle(theta2);
        phase2Tibias[i].SetAngle(theta3);
      }


    }
  

void tripodGaitBrute(){

  Joint phase1Coxas[3] = {L1_coxa,L3_coxa,L5_coxa};
  Joint phase1Femurs[3] = {L1_femur,L3_femur,L5_femur};
  Joint phase1Tibias[3] = {L1_tibia,L3_tibia,L5_tibia};

  Joint phase2Coxas[3] = {L2_coxa,L4_coxa,L6_coxa};
  Joint phase2Femurs[3] = {L2_femur,L4_femur,L6_femur};
  Joint phase2Tibias[3] = {L2_tibia,L4_tibia,L6_tibia};

  
  int x = xyValues[0];
  int y = xyValues[1];
  if (x == 0) {
    if (y > 0) {
      //FOrward
      for(int i = 0; i<3;i++){
      if(isOnTheLeft(phase1Coxas[i]))
        phase1Coxas[i].setAngle(10);
      else phase1Coxas[i].setAngle(170);

      phase1Femurs[i].setAngle(60); // pulling this out of my ass!! Needs to be tested!!!
      phase1Tibias[i].setAngle(120);

    }
    delay(300); 
    for(int i = 0; i<3;i++){
      if(isOnTheLeft(phase1Coxas[i]))
        phase1Coxas[i].setAngle(10);
      else phase1Coxas[i].setAngle(170);

      phase1Femurs[i].setAngle(60); // pulling this out of my ass!! Needs to be tested!!!
      phase1Tibias[i].setAngle(120);

    }

    } else {
     // "Backward";
    }
  } else if (y == 0) {
    if (x > 0) {
      // "Right";
    } else {
      // "Left";
    }
  } else {
    if (y > 0) {
      if (x > 0) {
        // "Forward-Right";
      } else {
        // "Forward-Left";
      }
    } else {
      if (x > 0) {
        // "Backward-Right";
      } else {
        // "Backward-Left";
      }
    }
  }

}

void waveGaitBrute(){


}

bool isOnTheLeft(Joint joint){

  Joint leftSidedJoints[9] = {L4_coxa,L4_femur,L4_tibia,
                              L5_coxa,L5_femur,L5_tibia,
                              L6_coxa,L6_femur,L6_tibia};

  for(int i =0; i<9;i++){
  if(joint.channel == leftSidedJoints[i].channel && joint.pwm == leftSidedJoints[i].pwm)
  return true;
  }
  return false;


}




