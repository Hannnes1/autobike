from param import *
import serial, time, numpy
# from pyvesc import VESC
import pyvesc
import pysnooper
import pyvisa
import signal
import threading
import sys
# @pysnooper.snoop()
class DriveMotor(object):
    serial = None

    def __init__(self, session):
        self.rm = pyvisa.ResourceManager()
        # self.instr = self.rm.open_resource('ASRL1::INSTR')
        self.instr = self.rm.open_resource('ASRL2::INSTR')  # change back
        self.instr.baud_rate = driveMotor_CommunicationFrequency

        # self.VESCmotor = VESC(serial_port=driveMotor_port,baudrate=driveMotor_CommunicationFrequency,start_heartbeat=False)

        # Start heartbeat threat
        self.heart_beat_thread = threading.Thread(target=self._heartbeat_cmd_func, daemon=True)
        self._stop_heartbeat = threading.Event()
        self.heart_beat_thread.start()

        if debug:
            print('Drive Motor : Serial port opened')
        self.Time = -1 # Initialize time to -1 as a way to check that we correctly read from the controller after starting the logging

    def _heartbeat_cmd_func(self):
        """
        Continuous function calling that keeps the motor alive
        """
        while not self._stop_heartbeat.isSet():
            time.sleep(0.1)
            self.instr.write_raw(pyvesc.messages.setters.alive_msg)

    def stop_heartbeat(self):
        self._stop_heartbeat.set()
        self.heart_beat_thread.join()

    def rear_set_rpm(self, rpm):
        # self.VESCmotor.set_rpm(int(rpm))
        self.instr.write_raw(pyvesc.protocol.interface.encode(pyvesc.messages.setters.SetRPM(int(rpm))))

    def set_velocity(self, input_velocity):
        # self.rear_set_pwm(self._m_per_second_to_pwm(input_velocity))
        self.rear_set_rpm(input_velocity*600) # Maxime's previous setting, might work for RED bike
        # self.rear_set_rpm(input_velocity * 1300)
        # print('VESC : Set speed')
        # self.VESCmotor.start_heartbeat()
        # for GEAR 6Th the coef vel -> pwm = 0.31
        # self.rear_set_rpm(input_velocity * 0.31)

    def stop(self):
        print('VESC : Stop')
        self.set_velocity(0)
        self.stop_heartbeat()
