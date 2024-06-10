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
#define BackLegs Serial6
#define FrontLegs Serial7


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


// Function to map a value from one range to another with floats
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Setup function
void setup()
{
  Serial.begin(115200);            // Boot up the serial monitor
  FrontLegs.begin(1000000);           // Boot up the serial port for the front legs
  BackLegs.begin(1000000);            // Boot up the serial port for the back legs
  m1.setGains(3.0, 0.0, 1.0);      // Set PID gains for m1
  m2.setGains(3.0, 0.0, 1.0);      // Set PID gains for m2
  m3.setGains(3.0, 0.0, 1.0);      // Set PID gains for m3
  m4.setGains(3.0, 0.0, 1.0);      // Set PID gains for m4
  ppi.begin(PPM_IN_PIN); // Initialize the pulse position input object


}
float kneeDesPos = 0;
float maxKneeVel = 0.1;

// Main loop
void loop()
{
  // First: if safety switch is detected, kill wheel motors and send kill command to slaves
  if(ppi.read(5)<1500){
    Serial.println("Safety on!");
    m1.kill();
    m2.kill();
    m3.kill();
    m4.kill();
    FrontLegs.println(0);
    FrontLegs.println(0.0);
    BackLegs.println(0);
    BackLegs.println(0.0);
  }

  // If safety switch is not detected, run normal control
  else if (ppi.read(5)>1500){
    // Read the joystick values and map them to the desired wheel duty cycles
    int FB_RJ = map(ppi.read(1), 1000, 2000, -255, 255);
    int LR_RJ = map(ppi.read(2), 1000, 2000, -255, 255);

    // Read ch6 knob and map to hip angles
    float HipSplay = mapfloat(ppi.read(6), 1000, 2000, -20, 80);
    BackLegs.println(4);
    BackLegs.println(HipSplay, 2);
    BackLegs.println(1);
    BackLegs.println(-HipSplay, 2);

    FrontLegs.println(4);
    FrontLegs.println(-HipSplay, 2);
    FrontLegs.println(1);
    FrontLegs.println(HipSplay, 2);

    // Read right joystick and map to knee angles
    int leftWheel = -FB_RJ + LR_RJ;
    int rightWheel = -FB_RJ - LR_RJ;

    //Constrain duty cycles to -255 to 255
    leftWheel = constrain(leftWheel, -255, 255);
    rightWheel = constrain(rightWheel, -255, 255);

    // Map left joystick to knee velocities:
    float kneeVel = mapfloat(ppi.read(3),1000,2000,0,maxKneeVel);
    float kneeLR = mapfloat(ppi.read(4),1000,2000,-maxKneeVel/2,maxKneeVel/2);
    if(ppi.read(3)<1010){
      kneeVel = 0;
    }

    kneeDesPos+=kneeVel;
    if(kneeLR<0){    
      BackLegs.println(2);
      BackLegs.println(kneeVel-kneeLR, 2);
      FrontLegs.println(2);
      FrontLegs.println(kneeVel-kneeLR, 2);
      BackLegs.println(3);
      BackLegs.println(-kneeVel, 2);
      FrontLegs.println(3);
      FrontLegs.println(-kneeVel, 2);
      }
    if(kneeLR>0){
      BackLegs.println(3);
      BackLegs.println(-kneeVel-kneeLR, 2);
      FrontLegs.println(3);
      FrontLegs.println(-kneeVel-kneeLR, 2);
      BackLegs.println(2);
      BackLegs.println(kneeVel, 2);
      FrontLegs.println(2);
      FrontLegs.println(kneeVel, 2);
      }

    // Drive the motors
    if(leftWheel<0){
      m1.fwdDrive(leftWheel);
      m2.fwdDrive(leftWheel);
    }
    else if(leftWheel>0){
      m1.revDrive(abs(leftWheel));
      m2.revDrive(abs(leftWheel));
    }
    
    if(rightWheel<0){
      m3.fwdDrive(rightWheel);
      m4.fwdDrive(rightWheel);
    }
    else if(rightWheel>0){
      m3.revDrive(abs(rightWheel));
      m4.revDrive(abs(rightWheel));
    }

  }
    
}

