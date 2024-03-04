#include <Motor.hpp>

Motor::Motor()
{
    this->EN_ = 2;
    this->DIR_ = 3;
    this->ENCA_ = 0;
    this->ENCB_ = 0;
    pinMode(EN_, OUTPUT);
    pinMode(DIR_, OUTPUT);
}

Motor::Motor(int EN, int DIR, int ENCA, int ENCB){

    this->EN_ = EN;
    this->DIR_ = DIR;
    this->ENCA_ = ENCA;
    this->ENCB_ = ENCB;
    pinMode(EN_, OUTPUT);
    pinMode(DIR_, OUTPUT);
    encoder_ = new Encoder(ENCA_, ENCB_);
}

Motor::~Motor(){
    delete encoder_;
}


void Motor::init(int INA, int INB, int ENCA, int ENCB)
{
    EN_ = INA;
    DIR_ = INB;
    ENCA_ = ENCA;
    ENCB_ = ENCB;
    pinMode(EN_, OUTPUT);
    pinMode(DIR_, OUTPUT);
    Encoder encoder(ENCA_, ENCB_);
}

void Motor::fwdDrive(int dutyCycle)
{
    int EN = this->EN_;   // The enable pin
    int DIR = this->DIR_; // The direction pin
    if (reverse_)
    {
        digitalWrite(DIR, LOW); // Set the direction
    }
    else
    {
        digitalWrite(DIR, HIGH); // Set the direction
    }
    analogWrite(EN, dutyCycle); // Set the speed
}

void Motor::revDrive(int dutyCycle)
{
    int EN = this->EN_;   // The enable pin
    int DIR = this->DIR_; // The direction pin
    if (reverse_)
    {
        digitalWrite(DIR, HIGH); // Set the direction
    }
    else
    {
        digitalWrite(DIR, LOW); // Set the direction
    }
    analogWrite(EN, dutyCycle); // Set the speed
}

// Only to be used to fix the direction of the motor if it is reversed
void Motor::setReverse()
{
    this->reverse_ = 0;
}

int Motor::motorPos()
{
    return encoder_->read();
}

float Motor::shaftPos()
{
    float pos = this->motorPos();
    return pos*gearReduction_;
}

void Motor::setGains(float Kp, float Ki, float Kd)
{
    Kp_=Kp;
    Ki_=Ki;
    Kd_=Kd;
}

// Position control function
void Motor::posControl(float desPos)
{
    float pos = this->shaftPos();
    shaftVel_ = pos-prevShaftPos_;

    
    float error = desPos - shaftPos();
    float velError = 0 - shaftVel_;
    int u = Kp_*error + Kd_*velError;
    u = constrain(u, -255, 255);
    if(u>=0){
        fwdDrive(u);
    }
    else{
        revDrive(u);
    }
    this->prevShaftPos_ = pos;
}