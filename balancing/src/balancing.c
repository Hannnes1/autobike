#include "balancing.h"

extern double stabilizeBike(double rollRate, double Kp, double Ki, double Kd) {
    double steeringRate = pid(0, rollRate, Kp, Ki, Kd);

    // Send Steering Rate Reference value to steering motor controller
    double steeringPWM = calculateSteeringPWM(steeringRate);

    return steeringPWM;
}

double calculateSteeringPWM(double angularVelocity) {
    double rpm = -angularVelocity * 30 / pi * gearRatio;  // To comply with Right Hand Rule
    return 50 + rpm * 40.0 / 1000.0;
}

double pid(double reference, double currentValue, double Kp, double Ki, double Kd) {
    double error = reference - currentValue;

    integral += error;
    derivative = error - previousError;

    if (integral < -windupGuard) {
       integral = -windupGuard;
    } else if (integral > windupGuard) {
        integral = windupGuard;
    }

    previousError = error;

    return Kp * error + Ki * integral + Kd * derivative;
}
