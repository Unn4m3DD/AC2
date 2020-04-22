#include <detpic32.h>

volatile int measured_duty_cycle = 0;
volatile int target_duty_cycle = 50;
void setPWM1(unsigned int dutyCycle, unsigned int PRx) {
  OC1RS = (PRx * dutyCycle) / 100;
}

void configPWM() {
  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 0;
  OC1RS = 62500 / 2;
  OC1CONbits.ON = 1;
}

void configT1() {
  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;
  T1CONbits.TCKPS = 5;
  PR1 = 62500;
  TMR1 = 0;
  T1CONbits.TON = 1;
}
void configT2() {
  IPC2bits.T2IP = 0;
  IEC0bits.T2IE = 0;
  IFS0bits.T2IF = 0;
  T2CONbits.TCKPS = 4;
  PR2 = 65535;
  TMR2 = 0;
  T2CONbits.TON = 1;
}
void configRB5() { TRISBbits.TRISB5 = 1; }

int main() {
  configT1();
  configT2();
  configRB5();
  configPWM();
  setPWM1(target_duty_cycle, 65535);
  EnableInterrupts();
  while (1)
    ;
}

void _int_(4) isr_T1(void) {
  int clocks = 0;
  int up_clocks = 0;
  while (PORTBbits.RB5 == 0)
    ;
  resetCoreTimer();
  while (PORTBbits.RB5 == 1) {
    clocks += readCoreTimer();
    resetCoreTimer();
  }
  up_clocks = clocks;
  while (PORTBbits.RB5 == 0) {
    clocks += readCoreTimer();
    resetCoreTimer();
  }
  measured_duty_cycle = (up_clocks * 100) / (clocks * 100);
  printf("target_duty_cycle: %d, measured_duty_cycle: %d\n", target_duty_cycle,
         measured_duty_cycle);

  target_duty_cycle++;
  target_duty_cycle %= 100;
  setPWM1(target_duty_cycle, 65535);
  IFS0bits.T1IF = 0;
}
