#include <Stepper.h>
#include "max6675.h"

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

#define stire 6
#define heater 12
#define pump 7

#define thermoDO 13
#define thermoCS 3
#define thermoCLK 2

const int steps_per_rev = 200;

Stepper nemaMotor(steps_per_rev,IN1,IN2,IN3,IN4);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  Serial.begin(9600);

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);

  pinMode(stire,OUTPUT);
  pinMode(heater,OUTPUT);
  pinMode(pump,OUTPUT);

  digitalWrite(stire,HIGH);
  digitalWrite(heater,HIGH);
  digitalWrite(pump,HIGH);
  
  nemaMotor.setSpeed(60);
  reaction();

}

void loop() {
  
}

void reaction(){
  while(readTemp()<= 50.00){
    Serial.println(readTemp());
    Serial.println("below 100");
    controlStire(1);
    controlHeater(1);  
    }
  controlHeater(0);

  long int t1 = millis();
  while((millis()-t1)<=60000){
    Serial.println("equal to 100");
    Serial.println(readTemp());
    if(readTemp()< 50.00){
      controlHeater(1);  
    }else if(readTemp()>= 50.00){
      controlHeater(0);
    }  
  }

  controlHeater(0);

  while(readTemp()>=40.00){
    Serial.println("to 25");
    Serial.println(readTemp());
      controlStire(1);
  }

  
  Serial.println("Stepper on!");
  long int t3 = millis();
  while((millis()-t3)<7000){
    nemaMotor.step(steps_per_rev);  
  }
  setStepperIdle();
  

  controlPump(1);
  Serial.println("Pump on!");
  delay(10000);
  controlPump(0);

  while(readTemp()<= 50.00){
    Serial.println(readTemp());
    Serial.println("to 100");
    controlStire(1);
    controlHeater(1);  
   }
  controlHeater(0);

  long int t2 = millis();
  while((millis()-t2)<= 60000){
    Serial.println(readTemp());
    Serial.println("Waiting for 10mins");
    if(readTemp()< 50.00){
      controlHeater(1);  
    }else if(readTemp()>= 50.00){
      controlHeater(0);
    }  
  }

  controlHeater(0);
  controlStire(0);
  Serial.println("Finish");
  return;
  }

void controlStire(int sta){
  if(sta == 1){
    digitalWrite (stire,LOW);
  }else if(sta == 0){
    digitalWrite (stire,HIGH);
    }  
  delay(10);
}

void controlHeater(int sta){
  if (sta == 1){
    digitalWrite (heater,LOW);
   }else if(sta == 0){
    digitalWrite (heater,HIGH);
    }
   delay(10);
  }

double readTemp(){
  double temp = thermocouple.readCelsius();
  delay(2000);
  return temp;
  }

void controlPump(int sta){
  if(sta == 1){
    digitalWrite(pump,LOW);
    delay(10);
    }else if(sta == 0){
      digitalWrite(pump,HIGH);
      delay(10);
      }
  }
void setStepperIdle() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


