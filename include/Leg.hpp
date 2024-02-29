#include <Encoder.h>



class Leg {
public:
    // Constructor
    Leg();

    // Encoders:
    int kneeEncPinA = 2;
    int kneeEncPinB = 3;
    int hipEncPinA = 4;
    int hipEncPinB = 5;
    Encoder kneeEnc_ = Encoder(kneeEncPinA, kneeEncPinB);
    Encoder hipEnc_ = Encoder(hipEncPinA, hipEncPinB);

    // Member functions
    float desKneePos_ = 0; // Desired knee position
    float desKneeVel_ = 0; // Desired knee velocity

    float desHipPos_ = 0; // Desired hip position
    float desHipVel_ = 0; // Desired hip velocity

    // Motor State Variables:
    float kneePos_ = 0; // Knee position
    float kneeVel_ = 0; // Knee velocity
    float hipPos_ = 0; // Hip position
    float hipVel_ = 0; // Hip velocity

    // Motor PD Control Variables:
    float kneeKp_ = 0.1; // Knee proportional gain
    float kneeKd_ = 0.1; // Knee derivative gain
    float hipKp_ = 0.1; // Hip proportional gain
    float hipKd_ = 0.1; // Hip derivative gain


    // Axis-axis link lengths
    float L1_ = 0.65;
    float L2_ = 0.8;

    // Gear ratios
    float kneeGR_ = (1)/(380);
    float hipGR_ = (1)/(380);

    void setLegLengths(float l1, float l2); // Set the leg lengths
    void setDesKneePos(float pos); // Set the desired knee position
    void setKneeEncoderPins(int pinA, int pinB); // Set the knee encoder pins
    void setHipEncoderPins(int pinA, int pinB); // Set the hip encoder pins

    void controlLeg(); // Control the leg 



private:
    // Member variables


};
