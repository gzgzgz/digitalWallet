#!/bin/bash -f
cd src
make
mv run.x ..
cp *.in ..
cd ..

./run.x < ./para1.in
./run.x < ./para2.in
./run.x < ./para3.in

