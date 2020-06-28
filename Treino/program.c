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

void display(int val, int base)
{
  static unsigned int codes[] = {
      0b00111111, //0
      0b00000110, //1
      0b01011011, //2
      0b01001111, //3
      0b01100110, //4
      0b01101101, //5
      0b01111101, //6
      0b00000111, //7
      0b01111111, //8
      0b01101111,  //9
      0b01110111,  //A
      0b01111100,  //Bb
      0b00111001,  //C
      0b01011110,  //Dd
      0b01111001,  //E
      0b01110001   //F
                  //  01234567
  };
  static unsigned char currentDisplay = 0;
  if (currentDisplay == 0)
  {
    LATB = (LATB & 0x00ff) | (codes[(val / base) % base] << 8);
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    currentDisplay = 1;
  }
  else
  {
    LATB = (LATB & 0x00ff) | (codes[val % base] << 8);
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    currentDisplay = 0;
  }
}

/*
  7
2   6
  1
3   5
  4  0
*/

void setupDisplay()
{
  TRISB &= 0xf00ff;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
}

int main()
{
  setupDisplay();

  int *trisE = (int *)(0xBF886100);
  int trisEContent = *trisE;
  trisEContent &= 0xf0;
  *trisE = trisEContent;

  int res = 0;
  char actualC = '0';
  char c;
  while (1)
  {
    volatile int *latE = (int *)(0xBF886120);
    c = inkey();
    if (c != '\0')
    {
      actualC = c;
      switch (c)
      {
      case '0':
        res = 0b0001;
        break;
      case '1':
        res = 0b0010;
        break;
      case '2':
        res = 0b0100;
        break;
      case '3':
        res = 0b1000;
        break;
      default:
        res = 0b1111;
        break;
      }
    }
    *latE = (*latE & 0xf0) | res;
    if (res == 0b1111)
    {
      *latE &= 0xf0;
      int i;
      for (i = 0; i < 200; i++)
      {
        display(0xff, 16);
        delay(5);
      }
      actualC = '0';
      res = 0;
    }
    display(actualC - '0', 16);
    delay(5);
  }
  return 0;
}
