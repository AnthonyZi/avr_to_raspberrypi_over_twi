#include "pi_twi_slave.h"

twi_s* twi_init(uint8_t scl, uint8_t sda, uint8_t address, void (*twi_callback)(uint8_t*,uint8_t))
{
    twi_s *twi = (twi_s*)malloc(sizeof(twi_s));
    twi->scl = scl;
    twi->sda = sda;
    twi->address = address;
    twi->status = 0;
    twi->data = (uint8_t*)malloc(sizeof(uint8_t));
    twi->twi_callback = twi_callback;

    pinMode(twi->scl, INPUT);
    pinMode(twi->sda, INPUT);
    pullUpDnControl(twi->scl, PUD_UP);
    pullUpDnControl(twi->sda, PUD_UP);

    return twi;
}

void twi_listen(twi_s *twi)
{
    uint32_t counter = 0;
    while(1)
    {
        if(twi->status == 0)        //wait_start
        {
            twi_wait_start(twi);
            twi_wait_scl_pull(twi);
            twi->status = 1;
        }
        if(twi->status == 1)        //address_check
        {
            twi_receive_byte(twi);
        }
        while(twi->status >= 2)     //receive_mode
        {
            *(twi->data+(twi->data_counter++)) = twi_receive_byte(twi);
        }
        twi->twi_callback(twi->data,twi->data_counter);
    }
}

uint8_t twi_receive_byte(twi_s *twi)
{
    uint8_t i;
    uint8_t byte = 0;
    for(i=0; i<8; i++)
    {
        twi_wait_scl_release(twi);
        byte = (byte <<= 1) | digitalRead(twi->sda);
        switch(twi_wait_scl_pull(twi))
        {
            case 0:         // clock cycle
                break;
            case 1:         // START/REPEATED START BIT
                twi->status = 1;
                return 0;
            case 2:         // STOP BIT
                twi->status = 0;
                return 0;
            default:
                break;
        }
    }
    return twi_check_byte(twi, byte);
}

void twi_wait_start(twi_s *twi)
{
    while(1)
    {
        while(!(digitalRead(twi->scl) && digitalRead(twi->sda)));
        while(digitalRead(twi->scl) && digitalRead(twi->sda));
        if(digitalRead(twi->scl) && !digitalRead(twi->sda))
            return;
    }
}

uint8_t twi_wait_scl_pull(twi_s *twi)
{
    uint8_t begin_sda = digitalRead(twi->sda);
    while(digitalRead(twi->scl) == 1 && digitalRead(twi->sda) == begin_sda);
    if(digitalRead(twi->scl) == 0)
    {
        pinMode(twi->scl, OUTPUT);
        digitalWrite(twi->scl, 0);
        return 0;           // clock cycle
    }
    else if(begin_sda == 1)
    {
        return 1;           // START/REPEATED START BIT
    }
    else if(begin_sda == 0)
    {
        return 2;           // STOP BIT
    }
}

void twi_wait_scl_release(twi_s *twi)
{
    pinMode(twi->scl, INPUT);
    while(digitalRead(twi->scl) == 0);
    return;
}

uint8_t twi_check_byte(twi_s *twi, uint8_t byte)
{
    switch(twi->status)
    {
        case 1:             //address_check
            if((byte>>1) == twi->address)
            {
                twi_ack(twi);
                if((byte & 1) == 0)
                {
                    twi->status = 2;
                    twi->data_counter = 0;
                }
                else
                    twi->status = 3;
            }
            break;
        case 2:             //receive_mode
            twi_ack(twi);
            return byte;
            break;
        case 3:
            twi_ack(twi);
//            return byte;
        default:
            break;
    }
    return 0;
}

void twi_ack(twi_s *twi)
{
    pinMode(twi->sda, OUTPUT);
    digitalWrite(twi->sda, 0);
    twi_wait_scl_release(twi);
    twi_wait_scl_pull(twi);
    digitalWrite(twi->sda, 1);
    pinMode(twi->sda, INPUT);
}

void twi_nack(twi_s *twi)
{
    twi_wait_scl_release(twi);
    twi_wait_scl_pull(twi);
}
