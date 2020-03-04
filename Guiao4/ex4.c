#include <detpic32.h>
#include "./../helper.c"
int main() {
  TRISB &= 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
  int shift_amount = 0;
  while (1) {
    LATB = (LATB & 0x00FF) ^ (0x0100 << (shift_amount++));
    delay(10);
    LATD = LATD ^ 0x0060;
    LATB = (LATB & 0x00FF) ^ (0x0100 << (shift_amount++));
    shift_amount %= 9;
    delay(10);
  }

  return 0;
}
