# Low Level Wheel Leg Robot Control


## Description

Low level control for robot using BD65496MUV motor controllers, Pololu micro metal gearmotors, and Pololu micro encoders.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Contact](#contact)

## Installation
This project runs within the PlatformIO development environment for visual studio code. 

## Usage

### Motor Class:
Abstraction that allows you to define a motor object based on four pins. From here you can use the library to set gains, reverse the motor, etc. 

Use the following constructor:
`Motor myMotor(int EN,int DIR,int ENCA,intENCB);`

To set gains:
`myMotor.setGains(float Kp,float Ki,float Kd);`

To reverse motor direction:
`myMotor.setReverse();`

To set motor gear reduction:
`myMotor.setGearReduction(float GR);`


## License
This project is licensed under the [MIT License](LICENSE).

## Contact

If you have any questions or suggestions, feel free to reach out to me at [djay@fsu.edu]
