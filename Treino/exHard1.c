#include "detpic32.h"

void send2displays(int value)
{
  int codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0X7D, 0x07,
                 0X7F, 0X6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static int current_display = 0;
  if (current_display)
    LATB = (LATB & 0xf00ff) | (codes[value % 10] << 8);
  else
    LATB = (LATB & 0xf00ff) | (codes[value / 10 % 10] << 8);

  LATD ^= 0x0060;
  current_display = !current_display;
}
void delay(unsigned int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer() < PBCLK / 1000)
      ;
  }
}

void setupDisplays()
{
  TRISB &= 0xF00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
}

void setupLeds()
{
  TRISE &= 0xF0;
}

int main()
{
  LATD ^= 0x0060;
  int bClk = 400, wClk = 400, numDisp;
  unsigned char current = 'w';
  setupLeds();
  setupDisplays();
  while (1)
  {
    char in = inkey();
    if (in == 'x')
      break;
    else if (in == 'b' || in == 'w')
      current = in;

    if (current == 'w')
    {
      numDisp = --wClk % 100;
      LATEbits.LATE0 = wClk / 100 >= 1;
      LATEbits.LATE1 = wClk / 100 >= 2;
      LATEbits.LATE2 = wClk / 100 >= 3;
      LATEbits.LATE3 = wClk / 100 >= 4;
    }
    else if (current == 'b')
    {
      LATEbits.LATE0 = bClk / 100 >= 1;
      LATEbits.LATE1 = bClk / 100 >= 2;
      LATEbits.LATE2 = bClk / 100 >= 3;
      LATEbits.LATE3 = bClk / 100 >= 4;
      numDisp = --bClk % 100;
    }

    if (wClk <= 0 || bClk <= 0)
    {
      current = current == 'b' ? 'w' : 'b';
      break;
    }
    int i;
    for (i = 0; i < 50; i++)
    {
      send2displays(numDisp);
      delay(2);
    }
  }
  printStr(current == 'b' ? "Vitoria das Pretas!" : "Vitoria das Brancas!");
}
