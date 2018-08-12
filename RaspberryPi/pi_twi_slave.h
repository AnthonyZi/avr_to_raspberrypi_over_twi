#ifndef _PI_TWI_SLAVE_H_
#define _PI_TWI_SLAVE_H_

#include <stdint.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

#define TWI_FREQ        100000

typedef struct {
    uint8_t scl;
    uint8_t sda;

    uint8_t address;
    uint8_t status;     //0:wait_start, 1:address_check, 2:receive_mode
    uint8_t *data;
    uint8_t data_counter;
    void (*twi_callback)(uint8_t*,uint8_t);
} twi_s;

twi_s* twi_init(uint8_t scl, uint8_t sda, uint8_t address, void (*twi_callback)(uint8_t*,uint8_t));
void twi_listen(twi_s *twi);
uint8_t twi_receive_byte(twi_s *twi);
void twi_wait_start(twi_s *twi);
uint8_t twi_wait_scl_pull(twi_s *twi);
void twi_wait_scl_release(twi_s *twi);

uint8_t twi_check_byte(twi_s *twi, uint8_t byte);
void twi_ack(twi_s *twi);
void twi_nack(twi_s *twi);

#endif /* _PI_TWI_SLAVE_H_ */
