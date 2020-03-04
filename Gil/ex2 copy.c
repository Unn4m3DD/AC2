
#include <detpic32.h>

void send2displays(int high, int low, unsigned int ms);
void putDisplay(int res);

int main(void) {
  // before configuring the port as output
  // RD5 configured as output
  // RD6 configured as output
  TRISD = TRISD & 0XFF9F;
  TRISB = TRISB & 0x00FF;
  TRISB = TRISB ^ 0X000F;
  LATD = (LATD & 0xFF9F) | 0x0020;  // display high inactive, low  active

  putDisplay(0);
  int high;
  int low;
  // LATD = LATD ^ 0x0060;   toggle display selection (ex 9)
  while (1) {
    for (high = 0; high < 16; high++) {
      for (low = 0; low < 16; low++) {
        send2displays(high, low, 200);
        printf("%d %d\n", high, low);
      }
    }
  }

  return 0;
}

void delay(unsigned int ms) {
  int k = 20000;
  for (; ms > 0; ms--) {
    resetCoreTimer();
    while (readCoreTimer() < k)
      ;
  }
}
void send2displays(int high, int low, unsigned int ms) {
  int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0X7D, 0x07,
                          0X7F, 0X6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  putDisplay(display7Scodes[high]);
  LATD = LATD ^ 0x0060;
  delay(ms);
  putDisplay(display7Scodes[low]);
  LATD = LATD ^ 0x0060;
}

void putDisplay(int res) {
  LATB = LATB & 0x00FF;
  // printf("%c -> %d\n", c, res);
  res = res << 8;

  LATB = LATB | res;
}
