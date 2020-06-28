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

void setupTimer()
{
  T3CONbits.TCKPS = 5;
  PR3 = PBCLK / 32 / 10;
  TMR3 = 0;
  T3CONbits.TON = 1;

  IPC3bits.T3IP = 2;
  IFS0bits.T3IF = 0;
  IEC0bits.T3IE = 1;
}

//

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
void setupDisplay()
{
  TRISB &= 0xf00ff;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
}
volatile int counter = 0;

int freq = 10;
int main()
{
  setupDisplay();
  int frozenCounter = 0;
  setupTimer();
  EnableInterrupts();
  while (1)
  {
    int c = inkey();
    if ('0' <= c && c <= '4')
    {
      freq = 10 * (1 + c - '0');
    }
    printf("\r%2d", counter);

    if (c == '\n'){
      printf(" >> %2d", counter);
      frozenCounter = counter;
    }
    display(frozenCounter, 10);
    delay(5);
  }
  return 0;
}

void _int_(12) timer_isr()
{
  counter++;
  counter %= 100;
  PR3 = PBCLK / 32 / freq; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
  IFS0bits.T3IF = 0;
}
