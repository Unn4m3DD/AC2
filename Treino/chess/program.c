#include <detpic32.h>

void display(int val, int base)
{
  static int codes[] = {
      0b00111111, // 0
      0b00000110, // 1
      0b01011011, // 2
      0b01001111, // 3
      0b01100110, // 4
      0b01101101, // 5
      0b01111101, // 6
      0b00000111, // 7
      0b01111111, // 8
      0b01101111  // 9
  };
  static unsigned char currentDisplay = 0;
  if (currentDisplay == 0)
  {
    LATB = (LATB & 0xf00ff) | (codes[val / base % base] << 8);
    currentDisplay = 1;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
  }
  else
  {
    LATB = (LATB & 0xf00ff) | (codes[val % base] << 8);
    currentDisplay = 0;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
  }
}

void setupDisplay()
{
  TRISB &= 0xf00ff;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
}

void setupTimer()
{
  T2CONbits.TCKPS = 5;
  PR2 = 62499;
  TMR2 = 0;
  T2CONbits.TON = 1;
  IPC2bits.T2IP = 2;
  IEC0bits.T2IE = 1;
  IFS0bits.T2IF = 0;
}

void setupLeds()
{
  TRISE &= 0xf0;
}

void delay(int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer() < PBCLK / 1000)
      ;
  }
}

volatile int whiteCounter = 400, blackCounter = 400;
volatile unsigned char currentPlayer = 0;

void show()
{
  printf("\rJogador %s a jogar", currentPlayer ? "B" : "A");
  LATEbits.LATE3 = (currentPlayer ? whiteCounter : blackCounter) / 100 >= 4;
  LATEbits.LATE2 = (currentPlayer ? whiteCounter : blackCounter) / 100 >= 3;
  LATEbits.LATE1 = (currentPlayer ? whiteCounter : blackCounter) / 100 >= 2;
  LATEbits.LATE0 = (currentPlayer ? whiteCounter : blackCounter) / 100 >= 1;
  display((currentPlayer ? whiteCounter : blackCounter) % 100, 10);
}

int main()
{
  setupDisplay();
  setupTimer();
  setupLeds();
  EnableInterrupts();
  while (1)
  {
    char inChar = inkey();
    if (inChar == 'B' || inChar == 'b')
      currentPlayer = !currentPlayer;
    if (inChar == 'x' || inChar == 'X')
    {
      currentPlayer = !currentPlayer;
      break;
    }
    if ((currentPlayer ? whiteCounter : blackCounter) < 0)
      break;
    show();

    delay(2);
  }
  printf("\nVitória das %s", currentPlayer ? "Brancas" : "Pretas");

  return 0;
}

void _int_(8) timer2_isr()
{
  if (currentPlayer)
    whiteCounter--;
  else
    blackCounter--;
  IFS0bits.T2IF = 0;
}
