#include <detpic32.h>

#include "./../helper.c"
volatile int val = 0;
void configT1() {
  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;
  T1CONbits.TCKPS = 3;
  PR1 = 78121 / 4;
  TMR1 = 0;
  T1CONbits.TON = 1;
}

void configT3() {
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;
  T3CONbits.TCKPS = 5;
  PR3 = 6249;
  TMR3 = 0;
  T3CONbits.TON = 1;
}

void configADC() {
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
}

void configDisplay() {
  TRISB &= 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
}

void configSwitches() {
  TRISBbits.TRISB0 = 1;
  TRISBbits.TRISB1 = 1;
}

int main() {
  configT1();
  configT3();
  configADC();
  configDisplay();
  configSwitches();
  AD1CON1bits.ASAM = 1;  // startsample

  EnableInterrupts();
  while (1)
    ;
}

void _int_(4) isr_T1(void) {
  if (!(PORTBbits.RB0 == 1 && PORTBbits.RB1 == 0)) AD1CON1bits.ASAM = 1;
  IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void) {
  send2displaysDec(val);
  IFS0bits.T3IF = 0;
}

void _int_(27) isr_adc() {
  int *p = (int *)(&ADC1BUF0);
  int i = 0;
  int media = 0;
  for (i = 0; i < 4; i++) {
    media += (p[i * 4] * 33 + 511) / 1023;
  }
  media /= 4;
  IFS1bits.AD1IF = 0;
  LATEbits.LATE0 = 0;
  val = media;
  IFS1bits.AD1IF = 0;
}