#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
#include <PulsePosition.h>
using namespace TeensyTimerTool;
#include <Wire.h>

PulsePositionInput ppi;
#define PPM_IN_PIN 9


// Motor pin definitions
// NOT CORRECT YET
#define m1_EN 5
#define m1_DIR 6
#define m1_ENCA 2
#define m1_ENCB 7

#define m2_EN 3
#define m2_DIR 4
#define m2_ENCA 0
#define m2_ENCB 1

#define m3_EN 23
#define m3_DIR 22
#define m3_ENCA 18
#define m3_ENCB 19

#define m4_EN 15
#define m4_DIR 20
#define m4_ENCA 16
#define m4_ENCB 17


// Variables for holding desired motor positions:
float m1DesPos = 180;
float m2DesPos = 180;
float m3DesPos = 180;
float m4DesPos = 180;

// Create a timer object to control the control loop
PeriodicTimer controlTimer;

// Create a motor object for the knee motor
Motor m1 = Motor(m1_EN, m1_DIR, m1_ENCA, m1_ENCB);
Motor m2 = Motor(m2_EN, m2_DIR, m2_ENCA, m2_ENCB);
Motor m3 = Motor(m3_EN, m3_DIR, m3_ENCA, m3_ENCB);
Motor m4 = Motor(m4_EN, m4_DIR, m4_ENCA, m4_ENCB);

// 
void sendUpdate(){
  Wire.write("4");
  Wire.write("/");
  Wire.write("hello");
}

// Main control function to run every 5ms
void controlFunc(){
  //m1.posControl(m1DesPos);
  //m2.posControl(m2DesPos);
  //m3.posControl(m3DesPos);
  //m4.posControl(m4DesPos);
}


// Function to update the angle of a joint in a leg through I2C
void updateLeg(int leg_address, int joint, float angle){
  char b[8];
  dtostrf(angle, 4, 2, b);
  Wire2.beginTransmission(leg_address);
  Wire2.write(joint);
  Wire2.write('/');
  Wire2.write(b);
  Wire2.endTransmission();
}

// Function to run every time we receive a message from the master
// it will come in the form (joint/angle) ex: (1/180) means set desired hip
// angle to 180 degrees
void receiveEvent(int howMany) {

  int i = 0;  // i starts at 0 when we are reading the first piece of data
  String data[2] = {"",""}; // Create an array to store the data (two items)

  while (Wire.available()) { // loop through all but the last

    // i = 0 when we are reading the first piece of data
    if(i == 0){
      byte c = Wire.read(); // receive byte as a character
      // If we see the delimeter, move to the next piece of data
      if(c == '/'){
        i = 1;
        continue;
      } 
      // Add the character to the first data string
      data[i]+=c;
    }
    // i = 1 when we are reading the second piece of data
    // This time we are reading in the float value.
    if(i == 1){
      char c = Wire.read();
      data[i]+=c;
    }

  }
  int d1 = data[0].toInt();
  Serial.println(d1);
  float angVal = data[1].toFloat();
  Serial.println(angVal);
  Serial.println(millis());
}

// Setup function
void setup()
{
  // Wire.begin(80);                  // Join the I2C bus as a slave with address 80
  Serial.begin(115200);            // Boot up the serial monitor
  m1.setGains(3.0, 0.0, 1.0);      // Set PID gains for m1
  m2.setGains(3.0, 0.0, 1.0);      // Set PID gains for m2
  m3.setGains(3.0, 0.0, 1.0);      // Set PID gains for m3
  m4.setGains(3.0, 0.0, 1.0);      // Set PID gains for m4
  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms
  ppi.begin(PPM_IN_PIN); // Initialize the pulse position input object
  // Wire.onRequest(sendUpdate);
  // Wire.onReceive(receiveEvent);

}

// Main loop
void loop()
{
  Serial.println(ppi.read(5));

  // If safety switch is detected, kill motors.
  if(ppi.read(5)<1500){
    m1.kill();
    m2.kill();
    m3.kill();
    m4.kill();
    
  }
  // If safety switch is not detected, run normal control
  else if (ppi.read(5)>1500){
    
    int FB_RJ = map(ppi.read(1), 1000, 2000, -255, 255);
    int LR_RJ = map(ppi.read(2), 1000, 2000, -255, 255);

    int leftWheel = -FB_RJ + LR_RJ;
    int rightWheel = -FB_RJ - LR_RJ;

    //Constrain duty cycles to -255 to 255
    leftWheel = constrain(leftWheel, -255, 255);
    rightWheel = constrain(rightWheel, -255, 255);

    if(leftWheel<0){
      m1.fwdDrive(leftWheel);
      m4.fwdDrive(leftWheel);
    }
    else if(leftWheel>0){
      m1.revDrive(abs(leftWheel));
      m4.revDrive(abs(leftWheel));
    }
    
    if(rightWheel<0){
      m2.fwdDrive(rightWheel);
      m3.fwdDrive(rightWheel);
    }
    else if(rightWheel>0){
      m2.revDrive(abs(rightWheel));
      m3.revDrive(abs(rightWheel));
    }
  }
    



}

