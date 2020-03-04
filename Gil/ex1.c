#include <detpic32.h>

void delay(unsigned int ms);
void putDisplay(int res);

int main(void){
    // before configuring the port as output
    TRISDbits.TRISD5 = 0; // RD5 configured as output
    TRISDbits.TRISD6 = 0; // RD6 configured as output
    TRISD = TRISD & 0XFF9F;
    TRISB = TRISD & 0x00F0;
    TRISB = TRISB ^ 0X000F;
    LATD = (LATD & 0xFF9F) | 0x0020; // display high inactive, low  active 
    int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F ,0x66, 0x6D, 0X7D, 0x07, 0X7F, 0X6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
                    //    0     1     2     3     4     5    6      7     8    9     A     B     C     D     E     F
    putDisplay(0);
    int i = 0;
    //LATD = LATD ^ 0x0060;   toggle display selection (ex 9)
    while(1) {
        i = PORTB & 0x000F;
        putDisplay(display7Scodes[i]);
        LATD = LATD ^ 0x0060;
    }
     return 0;
}

void delay(unsigned int ms){
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}


void putDisplay(int res) {
        LATB = LATB & 0x00FF;
        //printf("%c -> %d\n", c, res);
        res = res << 8;

        LATB = LATB | res;
}


