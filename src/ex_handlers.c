#include "trace.h"

extern void hang();

void c_undefined_instruction(){
    uart_strln("Exception: undefined instruction");
    hang();
}

void c_prefetch_abort(){
    uart_strln("Exception: undefined instruction");
    hang();
}

void c_data_abort(){
    uart_strln("Exception: data abort");
    hang();
}

void c_unused(){
    uart_strln("Exception: unused");
    hang();
}

void c_fiq(){
    uart_strln("Exception: FIQ");
    hang();
}

