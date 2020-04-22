#include <detpic32.h>

void configT3() {
  T3CONbits.TCKPS = 5;
  PR3 = 6249;
  TMR3 = 0;
  T3CONbits.TON = 1;
}

void configPWM() {
  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 1;
  OC1RS = 6249 / 4;
  OC1CONbits.ON = 1;
}

int main() {
  configT3();
  configPWM();
  while (1)
    ;
}