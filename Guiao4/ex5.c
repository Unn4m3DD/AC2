#include "detpic32.h"
#include "./../helper.c"
int main() {
  TRISDbits.TRISD6 = 0;
  TRISDbits.TRISD5 = 0;
  LATDbits.LATD5 = 1;
  TRISB &= 0x00ff;
  while (1)
  {
    int index = PORTB & 0x000f;
    LATB = codes[index] << 8;
  }
  
}
