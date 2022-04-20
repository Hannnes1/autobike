#ifndef _BALANCING_H_
#define _BALANCING_H_

#define gearRatio 111.0
#define pi 3.141592

#define Kp 10.0
#define Ki 10.0
#define Kd 0.0
#define windupGuard 6

double integral = 0;
double derivative = 0;

double previousError = 0;

extern double stabilizeBike(double rollRate);

double calculateSteeringPWM(double angularVelocity);

double pid(double reference, double currentValue);

#endif