#include <detpic32.h>

int main() {
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;
  T3CONbits.TCKPS = 5;
  PR3 = 62499;
  TMR3 = 0;
  T3CONbits.TON = 1;

  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;
  T1CONbits.TCKPS = 3;
  PR1 = 78123 / 2;
  TMR1 = 0;
  T1CONbits.TON = 1;

  EnableInterrupts();
  while (1)
    ;
  return 0;
}

void _int_(4) isr_T1(void) {
  printf("1");
  IFS0bits.T1IF = 0;
}
void _int_(12) isr_T3(void) {
  printf("3");
  IFS0bits.T3IF = 0;
}