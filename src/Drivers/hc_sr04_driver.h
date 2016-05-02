#ifndef __HC_SR04_DRIVER_H__
#define __HC_SR04_DRIVER_H__

void init_sonar_driver();

float sonar_get_distance(int sensor_id);

#endif
