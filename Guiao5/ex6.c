#include <detpic32.h>
#include "./../helper.c"
int readAnalog() {
  LATEbits.LATE0 = 1;
  AD1CON1bits.ASAM = 1;
  while (IFS1bits.AD1IF == 0)
    ;
  LATEbits.LATE0 = 0;
  
  int *p = (int *)(&ADC1BUF0);
  int i = 0;
  int media = 0;
  for (i = 0; i < 4; i++) {
    media += (p[i * 4] * 33 + 511) / 1023;
  }
  media /= 4;
  IFS1bits.AD1IF = 0;
  return media;
}

int main() {
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 1;
  AD1CON2bits.SMPI = 4 - 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;

  TRISEbits.TRISE0 = 0;

  TRISB &= 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
  int val;
  int i = 0;
  while (1) {
    if (i++ % 25 == 0) val = readAnalog();
    send2displaysDec((unsigned char)val);
    delay(10);
  }
  return 0;
}
//delay de 7.3 us
