#!/bin/bash -f
cd src
compile.sh
mv run.x ..
cd ..

~/digitalWallet/run.x < ~/digitalWallet/para1.in
~/digitalWallet/run.x < ~/digitalWallet/para2.in
~/digitalWallet/run.x < ~/digitalWallet/para3.in

