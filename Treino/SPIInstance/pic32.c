#include <detpic32.h>

void delay(int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer() < PBCLK / 1000)
      ;
  }
}

int main()
{
  char msg[] = "Hello World";
  unsigned char pos = 0;
  TRISDbits.TRISD0 = 0; //clk
  TRISDbits.TRISD1 = 0; //out
  TRISDbits.TRISD2 = 1; //in
  LATDbits.LATD0 = 0;
  delay(1000);
  while (1)
  {
    char currentMsg = msg[pos++];
    pos %= 11;
    int i;
    for (i = 0; i < 8; i++)
    {
      LATDbits.LATD0 = 1;
      LATDbits.LATD1 = currentMsg & 1;
      currentMsg = currentMsg >> 1;
      delay(20);
      LATDbits.LATD0 = 0;
      delay(20);
    }
    LATDbits.LATD0 = 0;
    delay(200);
  }
  return 0;
}