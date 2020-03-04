#include <detpic32.h>

int main() {
  TRISB &= 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
  int* codes = {""};
  while (1) {
    char c = getChar();
    int shift_amount;
    if (65 <= c && c < 72)
      shift_amount = c - 65;
    else if (97 <= c && c < 104)
      shift_amount = c - 97;
    else if (c == '.')
      shift_amount = 7;

    LATB = (LATB & 0x00FF) ^ (0x0100 << shift_amount);
  }

  return 0;
}
