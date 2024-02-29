#include <Leg.hpp>


/// @brief Constructor
Leg::Leg(){
}

/// @brief Change leg lengths if necessary
/// @param l1 
/// @param l2 
void Leg::setLegLengths(float l1, float l2){
    Leg::L1_ = l1;
    Leg::L2_ = l2;
}


/// @brief Change the desired knee position
void Leg::setDesKneePos(float pos){
    Leg::desKneePos_ = pos;
}

/// @brief Allows the user to change the pins for the hip encoder
/// @param pinA 
/// @param pinB 
void Leg::setHipEncoderPins(int pinA, int pinB){
    Leg::hipEncPinA = pinA;
    Leg::hipEncPinB = pinB;
    Leg::hipEnc_ = Encoder(hipEncPinA, hipEncPinB);
}

/// @brief Allows the user to change the pins for the knee encoder
/// @param pinA
/// @param pinB
void Leg::setKneeEncoderPins(int pinA, int pinB){
    Leg::kneeEncPinA = pinA;
    Leg::kneeEncPinB = pinB;
    Leg::kneeEnc_ = Encoder(kneeEncPinA, kneeEncPinB);
}   