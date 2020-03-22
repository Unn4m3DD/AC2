#include <detpic32.h>

int main() {
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 1;
  AD1CON2bits.SMPI = 16 - 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;

  while (1) {
    AD1CON1bits.ASAM = 1;
    while (IFS1bits.AD1IF == 0)
      ;
    int *p = (int *)(&ADC1BUF0);
    int i;
    int media = 0;
    for (i = 0; i < 16; i++) {
      media += (p[i * 4] * 33 + 511) / 1023;
    }
    media /= 4;
    printf("%d ", media/4);

    printf("\n");
    IFS1bits.AD1IF = 0;
  }
  return 0;
}
