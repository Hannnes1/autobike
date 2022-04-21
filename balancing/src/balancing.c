#include "balancing.h"

extern double stabilizeBike(double rollRate) {
    double steeringRate = pid(0, rollRate);

    // Send Steering Rate Reference value to steering motor controller
    double steeringPWM = calculateSteeringPWM(steeringRate);

    // Right now, the PWM is between 10 and 90 (this should be scaled down by a factor of 100)
    // if the angular velocity is between -0.9435 and 0.9435.
    // That means that the error can be at most +- 0.04715 if integral is 0.
    // (With Kp = 10, Ki = 10, Kd = 0)
    // Changing to Kp = 0.2, Ki = 0, Kd = 0, the error can at most be +- 4.715.
    return steeringPWM;
}

// Is there any reason not to calculate the PWM directly in the pid function?
// How can we even be sure that the angularVelocity matches reality?
double calculateSteeringPWM(double angularVelocity) {
    double rpm = -angularVelocity * 30 / pi * gearRatio;  // To comply with Right Hand Rule
    return 50 + rpm * 40.0 / 1000.0;
}

double pid(double reference, double currentValue) {
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
