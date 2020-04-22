#include <detpic32.h>

#include "./../helper.c"

int main() {
  configUART1(115200, 'N', 8, 1);
  TRISBbits.TRISB0 = 0;
  while (1) {
    while (U1STAbits.TRMT == 1)
      ;
    resetCoreTimer();
    LATBbits.LATB0 = 1;
    my_puts("0123456789\n");
    LATBbits.LATB0 = 0;
    printf("%d", readCoreTimer() * 50);
  }
}