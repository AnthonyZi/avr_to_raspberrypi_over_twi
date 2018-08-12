//#include <inttypes.h>
//#include <stdio.h>
//#include <stdlib.h>
#ifndef F_CPU
#   define  F_CPU           8000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

#define  MAX_ATTEMPTS    200

void twi_init(uint8_t prescaler, uint8_t bitrate);

int twi_start();

int twi_address(uint8_t address, uint8_t write);

int twi_data(uint8_t *data, uint8_t len);

void twi_stop();

int twi_wait_for_twint();

int twi_write_data(uint8_t address, uint8_t *data, uint8_t len);
