#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;
#include <Wire.h>

// Motor pin definitions
// NOT CORRECT YET
#define m1_EN 23
#define m1_DIR 22
#define m1_ENCA 18
#define m1_ENCB 19

#define m2_EN 21
#define m2_DIR 20
#define m2_ENCA 16
#define m2_ENCB 17

#define m3_EN 15
#define m3_DIR 14
#define m3_ENCA 10
#define m3_ENCB 11

#define m4_EN 13
#define m4_DIR 12
#define m4_ENCA 6
#define m4_ENCB 7


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
  m1.posControl(m1DesPos);
  m2.posControl(m2DesPos);
  m3.posControl(m3DesPos);
  m4.posControl(m4DesPos);
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
  Wire.begin(80);                  // Join the I2C bus as a slave with address 80
  Serial.begin(115200);            // Boot up the serial monitor
  m1.setGains(3.0, 0.0, 1.0);      // Set PID gains for m1
  m2.setGains(3.0, 0.0, 1.0);      // Set PID gains for m2
  m3.setGains(3.0, 0.0, 1.0);      // Set PID gains for m3
  m4.setGains(3.0, 0.0, 1.0);      // Set PID gains for m4
  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms
  Wire.onRequest(sendUpdate);
  Wire.onReceive(receiveEvent);

}

// Main loop
void loop()
{
  m1DesPos = 180;
  m2DesPos = 180;
  m3DesPos = 180;
  m4DesPos = 180;
  delay(1000);
  m1DesPos = 0;
  m2DesPos = 0;
  m3DesPos = 0;
  m4DesPos = 0;
  delay(1000);
}

