#include "twi.h"
#include <util/delay.h>

int main()
{
    char* s = "abcdefghijklmnopqrstuvwxyz";

    while(1)
    {
        twi_init(2, 255); // prescaler 2, bitrate 255 -> ~1kHz SCL
        twi_write_data(0x4d,(uint8_t*)s,26);
//        _delay_ms(2000);
    }
}
