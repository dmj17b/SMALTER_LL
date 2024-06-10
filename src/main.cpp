#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
#include <PulsePosition.h>
using namespace TeensyTimerTool;
#include <Wire.h>

PulsePositionInput ppi;
#define PPM_IN_PIN 9


// Motor pin definitions
#define lefthip_EN 5
#define lefthip_DIR 6
#define lefthip_ENCA 2
#define lefthip_ENCB 7

#define rightknee_EN 3
#define rightknee_DIR 4
#define rightknee_ENCA 0
#define rightknee_ENCB 1

#define leftknee_EN 23
#define leftknee_DIR 22
#define leftknee_ENCA 18
#define leftknee_ENCB 19

#define righthip_EN 15
#define righthip_DIR 20
#define righthip_ENCA 16
#define righthip_ENCB 17

// I2C address for the legs:
#define Master Serial6


// Variables for holding desired motor positions:
float lefthipDesPos = 0;
float rightkneeDesPos = 0;
float leftkneeDesPos = 0;
float righthipDesPos = 0;
float rightkneeDuty = 0;
float leftkneeDuty = 0;
float leftkneeVel = 0;
float rightkneeVel = 0;

// Create a timer object to control the control loop
PeriodicTimer controlTimer;

// Create a motor object for the knee motor
Motor lefthip = Motor(lefthip_EN, lefthip_DIR, lefthip_ENCA, lefthip_ENCB);
Motor rightknee = Motor(rightknee_EN, rightknee_DIR, rightknee_ENCA, rightknee_ENCB);
Motor leftknee = Motor(leftknee_EN, leftknee_DIR, leftknee_ENCA, leftknee_ENCB);
Motor righthip = Motor(righthip_EN, righthip_DIR, righthip_ENCA, righthip_ENCB);


// Main control function to run every 5ms
void controlFunc(){
  lefthip.posControl(lefthipDesPos);
  righthip.posVelControl(righthipDesPos);
  leftknee.posVelControl(leftkneeDesPos);
  rightknee.posControl(rightkneeDesPos);
}



// Setup function
void setup()
{
  Serial.begin(115200);            // Boot up the serial monitor
  Master.begin(1000000
  );            // Boot up the serial monitor

  // Initialize the motor objects, set gains, and set gear reductions
  lefthip.setGains(4.0, 0.25, 2.0);      // Set PID gains for lefthip
  lefthip.setGearReduction((360*16)/(12*379.17*40));

  rightknee.setGains(4.0, 0, 2.0);      // Set PID gains for rightknee
  rightknee.setGearReduction((360*20)/(12*379.17*48));

  leftknee.setGains(4.0, 0, 2.0);      // Set PID gains for leftknee
  leftknee.setGearReduction((360*16)/(12*379.17*40));

  righthip.setGains(4.0, 0.25, 2.0);      // Set PID gains for righthip
  righthip.setGearReduction((360*20)/(12*379.17*48));

  // Start the control timer
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
    lefthip.kill();
    rightknee.kill();
    leftknee.kill();
    righthip.kill();
    controlTimer.stop();
    Serial.println("Killed all motors");
  }
  else{
    controlTimer.start();
  }

  // Either way, switch on the jointIndex to set the desired position of the motor
  switch(jointIndex){
    case 0:
      lefthip.kill();
      rightknee.kill();
      leftknee.kill();
      righthip.kill();
      Serial.println("Killed all motors");
    case 1:
      lefthipDesPos = input;
      break;
    case 2:
      rightkneeDesPos += input;
      break;
      
      break;
    case 3:
      leftkneeDesPos += input;
      break;

    case 4:
      righthipDesPos = input;
      break;
    default:
      lefthip.kill();
      rightknee.kill();
      leftknee.kill();
      righthip.kill();
      Serial.println("Killed all motors");
      break;
  }

}