#!/bin/sh

#Create a 3MB file

target=3145728  # 3MB
for j in `seq 1 10`
do
    for i in `seq 1 $target`
    do
        echo -n 1 >> medium$j.txt
    done
done
