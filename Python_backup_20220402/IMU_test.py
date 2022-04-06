from nifpga import Session
import time
import math
import re

gyro_sensitivity = 0.00875
accel_sensitivity = 0.000061
deg2rad = math.pi/180.0

# with Session("./FPGA Bitfiles/fpgaimugps_FPGATarget_FPGAimugps_lWpM7XqDZ5g.lvbitx", "RIO0") as session:
with Session("../FPGA Bitfiles/balancingnogps_FPGATarget_FPGAbalancingnoG_yn8cRoiCUew.lvbitx", "RIO0") as session:
    ###################################################################
    ############################### IMU ###############################
    ###################################################################
    fpga_GyroData = session.registers['SPI Read Gyro Data']
    fpga_AccData = session.registers['SPI Read Acc Data']

    # ###################################################################
    # ############################### GPS ###############################
    # ###################################################################
    # fpga_GPSData = session.registers['Read Data']

    t_start = time.time()

    while True:
        t_startLoop = time.time()

        t_start_IMU = time.time()
        ###################################################################
        ############################### IMU ###############################
        ###################################################################
        fpga_GyroData_value = fpga_GyroData.read()
        fpga_AccData_value = fpga_AccData.read()

        # Read gyro to buffer
        buf = fpga_GyroData_value
        buf = buf[:7]

        # Calculate gyro values from buffer
        gx = (buf[2] << 8) | buf[1]
        gy = (buf[4] << 8) | buf[3]
        gz = (buf[6] << 8) | buf[5]
        # Apply two's complement
        if (gx & (1 << (16 - 1))) != 0:
            gx -= (1 << 16)
        if (gy & (1 << (16 - 1))) != 0:
            gy -= (1 << 16)
        if (gz & (1 << (16 - 1))) != 0:
            gz -= (1 << 16)
        # # the PmodNav has y pointing to the right of x (left axised) but the bike has y pointing to the left of x (right axised)
        # gy  = -gy
        gx *= gyro_sensitivity * deg2rad
        gy *= gyro_sensitivity * deg2rad
        gz *= gyro_sensitivity * deg2rad
        # gx -= gx_offset
        # gy -= gy_offset
        # gz -= gz_offset

        # Read accel to buffer
        buf = fpga_AccData_value
        buf = buf[:7]

        # Calculate accel values from buffer
        ax = (buf[2] << 8) | buf[1]
        ay = (buf[4] << 8) | buf[3]
        az = (buf[6] << 8) | buf[5]
        # Apply two's complement
        if (ax & (1 << (16 - 1))) != 0:
            ax -= (1 << 16)
        if (ay & (1 << (16 - 1))) != 0:
            ay -= (1 << 16)
        if (az & (1 << (16 - 1))) != 0:
            az -= (1 << 16)
        # # the PmodNav has y pointing to the right of x (left axised) but the bike has y pointing to the left of x (right axised)
        # ay = -ay
        ax *= accel_sensitivity
        ay *= accel_sensitivity
        az *= accel_sensitivity

        print('IMU : t = %f ; gx = %f ; gy = %f ; gz = %f ; ax = %f ; ay = %f ; az = %f' %(time.time()-t_start,gx,gy,gz,ax,ay,az))
        t_end_IMU = time.time()

        t_start_GPS = time.time()
        # ###################################################################
        # ############################### GPS ###############################
        # ###################################################################
        # fpga_GPSData_value = fpga_GPSData.read()
        # nmea_str = ''.join(chr(int(i)) for i in fpga_GPSData_value)
        # nmea_str_split = re.split(',|\n|\r',nmea_str)
        #
        # try:
        #     if nmea_str_split[0] == '$GPRMC' or nmea_str_split[0] == '$GNRMC':
        #         rmc = nmea_str_split
        #         utc = rmc[1]
        #         status = rmc[2]
        #         latitude = rmc[3]
        #         NS_indicator = rmc[4]
        #         longitude = rmc[5]
        #         EW_indicator = rmc[6]
        #         Speed_over_Ground = rmc[7]
        #         Course_over_Ground = rmc[8]
        #         Date = rmc[9]
        #         Magnetic_Variation = rmc[10]
        #         Mode_rmc = rmc[11]
        #
        #         latitude = float(latitude)
        #         longitude = float(longitude)
        #
        #         print('GPS : t = %f ; lat = %f ; lon = %f ; NMEA_UTC = %s' % (time.time()-t_start,latitude,longitude,utc))
        #     else:
        #         print('Incorrect NMEA string received : ' + re.split('[\n\r]',nmea_str)[0])
        # except:
        #     print('Incorrect NMEA string received : ' + re.split('[\n\r]',nmea_str)[0])
        #
        # t_end_GPS = time.time()
        #
        # print('t_IMU = %f ; t_GPS = %f' % (t_end_IMU-t_start_IMU,t_end_GPS-t_start_GPS))
        # print('\n')

        time.sleep(0.1)