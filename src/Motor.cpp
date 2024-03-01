#include <Motor.hpp>

Motor::Motor(){
}

void Motor::fwdDrive(int dutyCycle){
    int INA = this->INA_;
    int INB = this->INB_;
    digitalWrite(INA, HIGH);
    analogWrite(INB, dutyCycle);
}

void Motor::init(int INA, int INB, int ENCA, int ENCB){
    this->INA_ = INA;
    this->INB_ = INB;
    this->ENCA = ENCA;
    this->ENCB = ENCB;
    pinMode(this->INA_, OUTPUT);
    pinMode(this->INB_, OUTPUT);
    Encoder encoder(this->ENCA, this->ENCB);
    this->encoder_ = encoder;

}

// Only to be used to fix the direction of the motor if it is reversed
void Motor::setReverse(){
    int tempA = this->INA_;
    this->INA_ = this->INB_;
    this->INB_ = tempA;
}