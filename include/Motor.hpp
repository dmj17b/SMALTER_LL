#include <Encoder.h>

class Motor{
public:
// Constructor
Motor();


int reverse_ = 1;
int INA_ = 2;
int INB_ = 3;

/// The pin numbers for the knee encoder
int ENCA = 0;
int ENCB = 0;
Encoder encoder_ = Encoder(ENCA, ENCB);

void setReverse(); // Set the motor to reverse direction for ALL commands
void fwdDrive(int speed); // Drive the motor forward
void init(int INA, int INB, int ENCA, int ENCB); // Initialize the motor
private:


};