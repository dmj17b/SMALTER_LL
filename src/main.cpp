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

// I2C address for the legs:
#define FrontLegs 80
#define BackLegs 90


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


// Main control function to run every 5ms
// (unused for wheel motors)
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

// Setup function
void setup()
{
  Wire2.begin();                  // Join the I2C bus as master
  Serial.begin(115200);            // Boot up the serial monitor
  m1.setGains(3.0, 0.0, 1.0);      // Set PID gains for m1
  m2.setGains(3.0, 0.0, 1.0);      // Set PID gains for m2
  m3.setGains(3.0, 0.0, 1.0);      // Set PID gains for m3
  m4.setGains(3.0, 0.0, 1.0);      // Set PID gains for m4
  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms
  ppi.begin(PPM_IN_PIN); // Initialize the pulse position input object

}

// Main loop
void loop()
{
  updateLeg(80, 1, 90);
  updateLeg(80, 2, 90);
  updateLeg(80, 3, 90);
  updateLeg(80, 4, 90);


  // If safety switch is detected, kill motors
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

