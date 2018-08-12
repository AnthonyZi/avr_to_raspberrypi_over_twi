#include "pi_twi_slave.h"

int x = 0;

void twi_callback(uint8_t* data, uint8_t len)
{
    printf("%d: ",++x);
    int i;
    for(i=0; i<len; i++)
    {
        printf("%c",(char)(*(data+i)));
    }
    printf("\n");
}

int main()
{
    printf("program_start\n");
    wiringPiSetup();
    twi_s *twi = twi_init(28,29,0x4d,twi_callback);
    twi_listen(twi);
    return 0;
}
