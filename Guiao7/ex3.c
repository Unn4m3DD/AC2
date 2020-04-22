#include <detpic32.h>

int main() {
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;
  T3CONbits.TCKPS = 7;
  PR3 = 78123 / 2;
  TMR3 = 0;
  T3CONbits.TON = 1;

  EnableInterrupts();
  while (1)
    ;
  return 0;
}

void _int_(12) isr_T3(void) {
  static int counter = 0;
  if (counter++ % 2 == 0) printf("%d", counter/2);
  IFS0bits.T3IF = 0;
}