import time

from param import *
from sensors import Encoder, HallSensor, IMU, SafetyStop, GPS, Klm_Estimator #, Potentiometer, DualLaserRanger
from actuators import DriveMotor, SteeringMotor
from controller import Controller
import pysnooper
from nifpga import Session

class Bike(object):
    # @pysnooper.snoop()
    def __init__(self, debug=True, recordPath=False, reverse=False, straight=False, path_file_arg='', rollref_file_arg='', steeringdist_file_arg='', simulate_file=''):

        with Session("../FPGA Bitfiles/balancingnogps_FPGATarget_FPGAbalancingV6_4.lvbitx","RIO0") as session:  # balancing Control V5

                # Initialize sensors and actuators
                if simulate_file == '':
                    self.safety_stop = SafetyStop(session)
                    self.encoder = Encoder(session)
                    self.hall_sensor = HallSensor(session)
                    self.imu = IMU(session, horizontal=False, Kalman = False)
                    if potentiometer_use:
                        self.potent = Potentiometer(session)
                    if gps_use:
                        self.gps = GPS(session)
                    if laserRanger_use:
                        self.laser_ranger = DualLaserRanger(session)
                    self.drive_motor = DriveMotor(session)
                    self.steering_motor = SteeringMotor(session)
                    if gps_use:
                        ini_gps_output = self.gps.get_latlon()
                        lat_0 = ini_gps_output[0]
                        lon_0 = ini_gps_output[1]
                        print([lat_0, lon_0])
                        self.Klm_Estimator = Klm_Estimator(lat_0, lon_0)

                # Run bike and controllers
                self.controller = Controller(self,recordPath,reverse,straight,path_file_arg,rollref_file_arg,steeringdist_file_arg,simulate_file)
                # self.controller.startup()
                self.controller.run()

    # if simulate_file == '':
    # Safety Stop
    def emergency_stop_check(self):
        return self.safety_stop.button_check()

    # Steering Encoder
    def get_handlebar_angle(self):
        return self.encoder.get_angle()

    # Hall Sensor
    def get_velocity(self):
        return self.hall_sensor.get_velocity()

    # IMU
    def get_imu_data(self, velocity, delta_state, phi):
        return self.imu.get_imu_data(velocity, delta_state, phi)

    # GPS
    if gps_use:
        def get_gps_data(self):
            return self.gps.get_position()

    # Laser Ranger
    if laserRanger_use:
        def get_laserRanger_data(self):
            return self.laser_ranger.get_y()

    # Potentiometer
    if potentiometer_use:
        def get_potentiometer_value(self):
            return self.potent.read_pot_value()

    # Drive Motor
    def set_velocity(self, input_velocity):
        self.drive_motor.set_velocity(input_velocity)

    # Steering Motor
    def set_handlebar_angular_velocity(self, angular_velocity):
        self.steering_motor.set_angular_velocity(angular_velocity)
        self.handlebar_angular_velocity = angular_velocity

    def get_handlebar_angular_velocity(self):
        return self.handlebar_angular_velocity

    # Stop bike
    def stop(self):
        self.steering_motor.stop()
        self.drive_motor.stop()

    def stop_all(self):
        self.stop()
