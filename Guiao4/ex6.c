#include <detpic32.h>
#include "./../helper.c"

int main() {
  TRISB &= 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
  int counter = 0;
  while (1) {
    int i;
    for (i = 0; i < 1000; i += 10) {
      send2displaysDec(counter);
      delay(10);
    }

    if (counter == 60) {
      for (i = 0; i < 500 * 10; i += 10) {
        if ((i % 1000) < 500) send2displaysDec(0);
        delay(10);
      }
      counter %= 60;
    }
    counter++;
  }
}