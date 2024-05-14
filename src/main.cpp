#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
#include <PulsePosition.h>
using namespace TeensyTimerTool;
#include <Wire.h>

PulsePositionInput ppi;
#define PPM_IN_PIN 9


// Motor pin definitions
#define leftHip_EN 5
#define leftHip_DIR 6
#define leftHip_ENCA 2
#define leftHip_ENCB 7

#define leftKnee_EN 3
#define leftKnee_DIR 4
#define leftKnee_ENCA 0
#define leftKnee_ENCB 1

#define rightHip_EN 23
#define rightHip_DIR 22
#define rightHip_ENCA 18
#define rightHip_ENCB 19

#define rightKnee_EN 15
#define rightKnee_DIR 20
#define rightKnee_ENCA 16
#define rightKnee_ENCB 17

// I2C address for the legs:
#define Master Serial6


// Variables for holding desired motor positions:
float leftHipDesPos = 0;
float leftKneeDesPos = 0;
float rightHipDesPos = 0;
float rightKneeDesPos = 0;

// Create a timer object to control the control loop
PeriodicTimer controlTimer;

// Create a motor object for the knee motor
Motor leftHip = Motor(leftHip_EN, leftHip_DIR, leftHip_ENCA, leftHip_ENCB);
Motor leftKnee = Motor(leftKnee_EN, leftKnee_DIR, leftKnee_ENCA, leftKnee_ENCB);
Motor rightHip = Motor(rightHip_EN, rightHip_DIR, rightHip_ENCA, rightHip_ENCB);
Motor rightKnee = Motor(rightKnee_EN, rightKnee_DIR, rightKnee_ENCA, rightKnee_ENCB);


// Main control function to run every 5ms
void controlFunc(){
  leftHip.posControl(leftHipDesPos);
  leftKnee.posControl(leftKneeDesPos);
  rightHip.posControl(rightHipDesPos);
  rightKnee.posControl(rightKneeDesPos);
}



// Setup function
void setup()
{
  Serial.begin(115200);            // Boot up the serial monitor
  Master.begin(9600);            // Boot up the serial monitor
  leftHip.setGains(4.0, 0.25, 2.0);      // Set PID gains for leftHip
  leftHip.setGearReduction((360*16)/(12*379.17*40));

  leftKnee.setGains(4.0, 0.25, 2.0);      // Set PID gains for leftKnee
  leftKnee.setGearReduction((360*20)/(12*379.17*48));

  rightHip.setGains(4.0, 0.25, 2.0);      // Set PID gains for rightHip
  rightHip.setGearReduction((360*16)/(12*379.17*40));

  rightKnee.setGains(4.0, 0.25, 2.0);      // Set PID gains for rightKnee
  rightKnee.setGearReduction((360*20)/(12*379.17*48));


  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms

}

int jointIndex;
// Main loop
void loop()
{

  // Very first thing to do is to check if there is any new data from the master
  static float input;
  while(Master.available()>0){
    // Parse data into jointIndex and input
    int d1 = Master.parseInt();
    input = Master.parseFloat();
    Serial.println(d1);
    jointIndex = d1;
    break;
  }

  // Check if the jointIndex is 0, if so, kill all motors
  if(jointIndex == 0){
    leftHip.kill();
    leftKnee.kill();
    rightHip.kill();
    rightKnee.kill();
    controlTimer.stop();
    Serial.println("Killed all motors");
  }
  else{
    controlTimer.start();
  }

  // Either way, switch on the jointIndex to set the desired position of the motor
  switch(jointIndex){
    case 0:
      leftHip.kill();
      leftKnee.kill();
      rightHip.kill();
      rightKnee.kill();
      Serial.println("Killed all motors");
    case 1:
      leftHipDesPos = input;
      break;
    case 2:
      leftKneeDesPos = input;
      break;
    case 3:
      rightHipDesPos = input;
      break;
    case 4:
      rightKneeDesPos = input;
      break;
    default:
      leftHip.kill();
      leftKnee.kill();
      rightHip.kill();
      rightKnee.kill();
      Serial.println("Killed all motors");
      break;
  }

}