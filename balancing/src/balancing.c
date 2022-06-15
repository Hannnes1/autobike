#include "balancing.h"

extern double stabilizeBike(double rollRate, double Kp, double Ki, double Kd) {
    double steeringRate = pid(0, rollRate, Kp, Ki, Kd);

    // Send Steering Rate Reference value to steering motor controller
    double steeringPWM = calculateSteeringPWM(steeringRate);

    return steeringPWM;
}

// Take in the wanted angular velocity of the handlebar,
// And return the required duty cycle for that velocity.
double calculateSteeringPWM(double angularVelocity) {
    // Convert from angular velocity (rad/s) of the handlebar,
    // to rpm of the motor.
    double rpm = -angularVelocity * 30 / pi * gearRatio;

    // Convert from rpm to duty cycle,
    // 4000 is the maximum speed of the motor (configured in Escon Studio).
    return 50 + rpm * 40.0 / 4000.0;
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
