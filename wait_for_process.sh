#!/bin/bash

PID=$1

while true
do
    ps -e | egrep --quiet "^$PID "
    if [ $? -ne 0 ]
    then
	exit
    fi
    sleep 1
done
