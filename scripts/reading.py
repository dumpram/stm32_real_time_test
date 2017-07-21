import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

num_samples = 1024
samples = np.empty(num_samples)

ser = serial.Serial(
        port='\\.\COMxx', # ovo tu prominiti da pase
        baudrate=baudrate,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        rtscts=0
    )

    if not ser.is_open:
        print("Error opening serial port device.")
        sys.exit(1)


    for i in range(num_samples):
        samples[i] = int(ser.readline().strip())

    plt.plot(samples)
