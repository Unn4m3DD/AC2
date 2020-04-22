#include <detpic32.h>

int main() {
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;
  T3CONbits.TCKPS = 7;
  PR3 = 78123 / 2;
  TMR3 = 0;
  T3CONbits.TON = 1;
  int counter = 0;
  while (1) {
    if (IFS0bits.T3IF == 1) {
      printf("%d", counter++);
      IFS0bits.T3IF = 0;
    }
  }

  return 0;
}