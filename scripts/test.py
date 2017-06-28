#!/usr/bin/python3

# System imports
import argparse
import sys
import serial

# Data processing imports
import numpy as np
import matplotlib.pyplot as plt

def checkparams(pwm_freq, pwm_duty, num_samples):
    check_ok = True
    if pwm_freq < 20 or pwm_freq > 100:
        print("Allowed PWM freq is between in [20, 100] kHz interval.")
        check_ok = False
    if pwm_duty < 5 or pwm_duty > 80:
        print("Allowed PWM duty is between in [5, 80] percent interval.")
        check_ok = False
    if num_samples < 1 or num_samples > 8192:
        print("Allowed samples num is between in [1, 8192] interval.")
        check_ok = False
    if check_ok == False:
        sys.exit(1);

def main(baudrate, pwm_freq, pwm_duty, num_samples):

    ser = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate=baudrate,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        rtscts=0
    )

    if not ser.is_open:
        print("Error opening serial port device.")
        sys.exit(1)

    checkparams(pwm_freq, pwm_duty, num_samples)

    print("Params OK!")

    delays = np.empty(num_samples)

    ser.write(str.encode('{},{},{}\r\n'.format(
        pwm_freq, 
        pwm_duty, 
        num_samples)))

    timer_frequency = int(ser.readline().strip()) # MHz
    ser.write(str.encode('\n')); # start measurement

    for i in range(num_samples):
        delays[i] = int(ser.readline().strip())

    ser.close()

    delays *= (1e-6 / timer_frequency);

    print("min: {}, avg: {}, max = {}".format(
        np.min(delays),
        np.mean(delays),
        np.max(delays)));

    print("std: ", np.std(delays))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--baudrate', type=int, default=115200)
    parser.add_argument('--pwm_freq', type=int, default=20)
    parser.add_argument('--pwm_duty', type=int, default=50)
    parser.add_argument('--num_samples', type=int, default=8192)

    ARGS, other = parser.parse_known_args()

    main(ARGS.baudrate, ARGS.pwm_freq, ARGS.pwm_duty, ARGS.num_samples);
