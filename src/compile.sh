#!/bin/bash -f
g++   -g -c  DigitalWallet.cpp -std=c++0x
g++   -g -c  digitalWalletTestMain.cpp -std=c++0x
g++   -g -c  TransLog.cpp -std=c++0x
g++   -g  -o   run.x digitalWalletTestMain.o DigitalWallet.o TransLog.o -std=c++0x 
rm -f *.o
