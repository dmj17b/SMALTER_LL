#include <Motor.hpp>

Motor::Motor(){
}


void Motor::init(int INA, int INB, int ENCA, int ENCB){
    this->EN_ = INA;
    this->DIR_ = INB;
    this->ENCA = ENCA;
    this->ENCB = ENCB;
    pinMode(this->EN_, OUTPUT);
    pinMode(this->DIR_, OUTPUT);
    Encoder encoder(this->ENCA, this->ENCB);
    this->encoder_ = encoder;

}

void Motor::fwdDrive(int dutyCycle){
    int EN = this->EN_;      // The enable pin
    int DIR = this->DIR_;   // The direction pin
    digitalWrite(DIR, HIGH*reverse_);   // Set the direction
    analogWrite(EN, dutyCycle); // Set the speed
}

void Motor::revDrive(int dutyCycle){
    int EN = this->EN_;     // The enable pin
    int DIR = this->DIR_;   // The direction pin
    digitalWrite(DIR, LOW*reverse_);   // Set the direction
    analogWrite(EN, dutyCycle); // Set the speed
}

// Only to be used to fix the direction of the motor if it is reversed
void Motor::setReverse(){
    this->reverse_ = 0;
}