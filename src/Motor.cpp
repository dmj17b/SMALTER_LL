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
    encoder_ = new Encoder(ENCA_, ENCB_);
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
    reverse_ = 0;
}

void Motor::setGearReduction(float gearReduction)
{
    gearReduction_ = gearReduction;
}

// Get encoder position
int Motor::motorPos()
{
    return encoder_->read();
}

// Get the position of the motor in degrees
float Motor::shaftPos()
{
    float pos = this->motorPos();
    return pos*gearReduction_;
}

// Set the PID gains
void Motor::setGains(float Kp, float Ki, float Kd)
{
    Kp_=Kp;
    Ki_=Ki;
    Kd_=Kd;
}

// PID position control function
void Motor::posControl(float desPos)
{
    float pos = this->shaftPos();       // Get the position of the output shaft
    shaftVel_ = pos-prevShaftPos_;      // Calculate the velocity of the output shaft (non-ideal, but good enough for now)
    float error = desPos - shaftPos();  // Calculate position error
    integralError_ += error;            // Increment the integral error
    float velError = 0 - shaftVel_;     // Calculate the velocity error

    int u = Kp_*error + Ki_*integralError_ + Kd_*velError;  // Calculate the control signal
    
    // Constrain the control signal to the maximum and minimum values
    if(u>255){
        u = 255;
        integralError_-=error; // Reset the integral error if the control signal is saturated
    }
    else if(u<-255){
        u = -255;
        integralError_-=error; // Reset the integral error if the control signal is saturated
    }

    if(abs(error)<1.0){
        integralError_ = 0; // Reset the integral error if the error is small
    }

    // If the control signal is positive, drive the motor forward
    if(u>=0){
        fwdDrive(u);    // Drive the motor
    }
    // If the control signal is negative, drive the motor in reverse
    else{
        revDrive(u);    // Drive the motor
    }
    prevShaftPos_ = pos;    // Update the previous position
}