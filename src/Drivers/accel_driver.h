#ifndef __ACCEL_DRIVER_H__
#define __ACCEL_DRIVER_H__

#include <stdint.h>

void init_accel_driver();

void get_accel_raw(int16_t acc[3]);
void get_accel(float acc[3]);

void get_turn_rate_raw(int16_t t[3]);
void get_turn_rate(float t[3]);

int16_t get_temp_raw();
float get_temp();

uint32_t get_irq_count();
uint32_t get_driver_count();

#endif
