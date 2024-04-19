#!/bin/sh

#Create a 3MB file

target=3145728  # 3MB
for i in `seq 1 $target`
do
    echo 1 >> med.txt
done