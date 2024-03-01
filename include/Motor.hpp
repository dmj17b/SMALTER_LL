#include <Encoder.h>

class Motor{
public:
// Constructor
Motor();


int reverse_ = 1;
int EN_ = 2;
int DIR_ = 3;

/// The pin numbers for the knee encoder
int ENCA = 0;
int ENCB = 0;
Encoder encoder_ = Encoder(ENCA, ENCB);

void setReverse(); // Set the motor to reverse direction for ALL commands
void fwdDrive(int speed); // Drive the motor forward
void revDrive(int speed); // Drive the motor in reverse
void init(int INA, int INB, int ENCA, int ENCB); // Initialize the motor


private:


};