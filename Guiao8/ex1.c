#include <detpic32.h>

int main() {
  U1BRG = 10;
  U1MODEbits.BRGH = 0;
  U1MODEbits.PDSEL = 0;
  U1MODEbits.STSEL = 0;
  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;
  U1MODEbits.ON = 1;
}