#!/bin/bash

echo "0" > /sys/class/gpio/gpio4/value && echo "1" > /sys/class/gpio/gpio4/value
sleep 3
cat $1 >/dev/ttyAMA0
echo "g" >/dev/ttyAMA0
