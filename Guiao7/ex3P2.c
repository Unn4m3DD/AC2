#include <detpic32.h>

#include "./../helper.c"
int main() {
  T3CONbits.TCKPS = 0;
  PR3 = 62499;
  TMR3 = 0;
  T3CONbits.TON = 1;

  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 1;
  OC1CONbits.ON = 1;
  int i = 0;
  int inc = 1;
  while (1) {
    if (i < 0 || i < 100) inc *= -2;
    if(inc > 50) inc = 1;
    i = i + inc;
    setPWM1(i, PR3);
    delay(200);
  }
}