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

    delays *= 1e6;

    plt.plot(delays)
    axes = plt.gca();
    axes.set_xlim([0,len(delays)])
    plt.ylabel('Vrijeme kašnjenja (${\mu}s$)', fontsize=20)
    plt.xlabel('Uzorci', fontsize=20)
    plt.show()

    plt.figure(0)
    n, bins, patches = plt.hist(delays, 50, normed=True, 
        histtype='step');

    y = mlab.normpdf(bins, 
        np.mean(delays), 
        np.std(delays))

    plt.show()
    plt.figure(1)
    plt.plot(bins, y)
    plt.xlabel('Vrijeme kašnjenja (${\mu}s$)', fontsize=20)
    plt.ylabel('Funkcija gustoće vjerojatnosti', fontsize=20)
    plt.show();

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--delays_file', type=str, default='novo.npy')

    ARGS, other = parser.parse_known_args()

    main(ARGS.delays_file);
