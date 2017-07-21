#!/usr/bin/python3

# System imports
import argparse
import sys
import serial

# Data processing imports
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import seaborn as sns
font = {'family' : 'normal',
        'weight' : 'bold'}

import matplotlib
matplotlib.rc('font', **font)

def main(delays_file):
    delays = np.load(delays_file)

    print("min: {}, avg: {}, max = {}".format(
    np.min(delays),
    np.mean(delays),
    np.max(delays)));

    print("std: ", np.std(delays))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--delays_file', type=str, default='novo.npy')

    ARGS, other = parser.parse_known_args()

    main(ARGS.delays_file);
