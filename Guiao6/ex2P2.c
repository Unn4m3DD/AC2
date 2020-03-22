#include <detpic32.h>
#include "./../helper.c"
volatile int val = 0;
int main() {
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 1;
  AD1CON2bits.SMPI = 4 - 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;

  IPC6bits.AD1IP = 2;
  IFS1bits.AD1IF = 0;
  IEC1bits.AD1IE = 1;

  TRISEbits.TRISE0 = 0;
  AD1CON1bits.ASAM = 1;  // startsample
  EnableInterrupts();
  while (1) {
    if (!IFS1bits.AD1IF) AD1CON1bits.ASAM = 1;  // startsample
    LATEbits.LATE0 = 0;
  }
}

void _int_(27) isr_adc() {
  int *p = (int *)(&ADC1BUF0);
  int i = 0;
  int media = 0;
  for (i = 0; i < 4; i++) {
    media += (p[i * 4] * 33 + 511) / 1023;
  }
  media /= 4;
  val = media;
  LATEbits.LATE0 = 1;
  IFS1bits.AD1IF = 0;
}
//delay do epilogo 1.06 us