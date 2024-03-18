#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;
#include <Wire.h>

#define kneeMotor_EN 23
#define kneeMotor_DIR 22
#define kneeMotor_ENCA 18
#define kneeMotor_ENCB 19


// Desired motor positions:
float kneeDesPos = 180;

// Create a timer object to control the control loop
PeriodicTimer controlTimer;



// Create a motor object for the knee motor
Motor kneeMotor = Motor(kneeMotor_EN, kneeMotor_DIR, kneeMotor_ENCA, kneeMotor_ENCB);

// 
void sendUpdate(){
  Wire.write("4");
  Wire.write("/");
  Wire.write("hello");
}

// Main control function to run every 5ms
void controlFunc(){
  kneeMotor.posControl(kneeDesPos);
}

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
  Wire.begin(80);                         // Join the I2C bus as a slave with address 80
  Serial.begin(115200);                   // Boot up the serial monitor
  kneeMotor.setGains(3.0, 0.0, 1.0);      // Set PID gains for kneeMotor
  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms
  Wire.onRequest(sendUpdate);
  Wire.onReceive(receiveEvent);

}

// Main loop
void loop()
{
}

