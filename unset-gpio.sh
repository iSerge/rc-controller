#!/bin/bash

echo "0" > /sys/class/gpio/gpio4/value
echo "in" > /sys/class/gpio/gpio4/direction
echo "4" > /sys/class/gpio/unexport
