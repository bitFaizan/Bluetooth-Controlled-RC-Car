#include "BluetoothSerial.h"


#define PWM_A 27
#define PWM_B 26
#define AIN1 13
#define AIN2 12
#define BIN1 14
#define BIN2 25

BluetoothSerial SerialBT;


char direction = 'S';
int speedLevel = 0;

int targetPWM = 0;
int currentPWM = 0;
const int pwmStep = 20 ;
const unsigned long accelDelay = 5;
unsigned long lastAccelTime = 0;

unsigned long timeout = 300;
unsigned long lastCommandTime = 0;


void setup() {
  Serial.begin(115200);

  SerialBT.begin("Pushbot 1");
  Serial.println("Bluetooth initiated. Pair with Pushbot 1");

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  ledcAttach(PWM_A,20000,8);
  ledcAttach(PWM_B,20000,8);

  stopMotors();
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();

    lastCommandTime = millis();
  
   if (command == 'F' || command == 'B' || command == 'R' ||
        command == 'L' || command == 'G' || command == 'H' ||
        command == 'I' || command == 'J' || command == 'S') {
      direction = command;
    }

    if ((command >= '0' && command <= '9') || command == 'q') {
      if (command == 'q') {
        speedLevel = 10;
      } else {
        speedLevel = command - '0';
      }
    }
  }

  if(millis() - lastCommandTime > timeout){
      direction ='S';
      speedLevel = 0 ;
  }

  targetPWM = map(speedLevel,0,10,0,255);
  targetPWM = constrain(targetPWM,0,255);

  if(direction == 'S'){
    targetPWM = 0;
  }

  if(millis() - lastAccelTime >= accelDelay){
    lastAccelTime = millis();

    if(currentPWM < targetPWM){
      currentPWM += pwmStep ;
      
      if(currentPWM > targetPWM){
        currentPWM = targetPWM;
      }
    }
    else if(currentPWM > targetPWM){
      currentPWM -= pwmStep ;

      if(currentPWM < targetPWM){
        currentPWM = targetPWM;
      }
    }

  }


  switch (direction) {
    case 'F': forward(currentPWM);
      break;
    case 'B': backward(currentPWM); 
      break;
    case 'R': right(currentPWM); 
      break;
    case 'L': left(currentPWM); 
      break;
    case 'G': forwardLeft(currentPWM); 
      break;
    case 'H': forwardRight(currentPWM); 
      break;
    case 'I': backwardLeft(currentPWM); 
      break;
    case 'J': backwardRight(currentPWM); 
      break;
  }
}


void forward(int speed) {
  analogWrite(PWM_B, speed);
  analogWrite(PWM_A, speed);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void backward(int speed) {
  analogWrite(PWM_A, speed);
  analogWrite(PWM_B, speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void right(int speed) {
  analogWrite(PWM_A, speed);
  analogWrite(PWM_B, speed);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void left(int speed) {
  analogWrite(PWM_A, speed);
  analogWrite(PWM_B, speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void forwardRight(int speed) {
  analogWrite(PWM_A, speed);
  analogWrite(PWM_B, speed / 2);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void forwardLeft(int speed) {
  analogWrite(PWM_A, speed / 2);
  analogWrite(PWM_B, speed);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void backwardRight(int speed) {
  analogWrite(PWM_A, speed);
  analogWrite(PWM_B, speed / 2);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void backwardLeft(int speed) {
  analogWrite(PWM_A, speed / 2);
  analogWrite(PWM_B, speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void stopMotors() {
  analogWrite(PWM_A, 0);
  analogWrite(PWM_B, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}
