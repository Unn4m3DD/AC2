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

void setupSwitch()
{
  TRISBbits.TRISB0 = 1;
  TRISBbits.TRISB1 = 1;
  TRISBbits.TRISB2 = 1;
  TRISBbits.TRISB3 = 1;
}

void setupADC()
{
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 16;
  AD1CON2bits.SMPI = 1 - 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;
  IPC6bits.AD1IP = 2;
  IEC1bits.AD1IE = 1;
  IFS1bits.AD1IF = 0;
}

void setupTimer2()
{
  T2CONbits.TCKPS = 1;
  PR2 = 50000;
  TMR2 = 0;
  T2CONbits.TON = 1;
  IPC2bits.T2IP = 3;
  IEC0bits.T2IE = 1;
  IFS0bits.T2IF = 0;
}

void display(int val)
{
  static char codes[] = {
      0b00111111, //0
      0b00000110, //1
      0b01011011, //2
      0b01001111, //3
      0b01100110, //4
      0b01101101, //5
      0b01111101, //6
      0b00000111, //7
      0b01111111, //8
      0b01101111  //9
  };
  static unsigned char currentDisplay = 0;
  if (currentDisplay)
    LATB = (LATB & 0xf00ff) | (codes[val / 10 % 10] << 8);
  else
    LATB = (LATB & 0xf00ff) | (codes[val % 10] << 8);
  LATDbits.LATD5 = !LATDbits.LATD5;
  LATDbits.LATD6 = !LATDbits.LATD6;
  currentDisplay = !currentDisplay;
}

void setupDisplay()
{
  TRISB = (TRISB & 0xf00ff);
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
}

void setupUart()
{
  int baudrate = 1200;
  U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
  U1MODEbits.STSEL = 0;
  U1MODEbits.PDSEL = 0;
  IEC0bits.U1RXIE = 1;
  IPC6bits.U1IP = 2;
  IFS0bits.U1RXIF = 0;
}

void printChar(char c)
{
  while (U1STAbits.UTXBF == 1)
    ;
  U1TXREG = c;
}

void printString(char *string)
{
  while ((*string) != '\0')
  {
    printChar(*string);
    string++;
  }
}

unsigned char buffer[16];
unsigned char first = 0, last = 0;

void printItem(unsigned char item)
{
  printString("\nRB3: ");
  printChar('0' + ((item & 0b1000) != 0));
  printString(" RB2: ");
  printChar('0' + ((item & 0b0100) != 0));
  printString(" RB1: ");
  printChar('0' + ((item & 0b0010) != 0));
  printString(" RB0: ");
  printChar('0' + ((item & 0b0001) != 0));
}

void printBuffer()
{
  int i = first;
  printItem(buffer[first]);
  i = (i + 1) % 16;
  for (; i != first; i = (i + 1) % 16)
  {
    printItem(buffer[i]);
  }
  delay(1);
}

volatile int freq = 1;
int main()
{
  setupSwitch();
  setupADC();
  setupTimer2();
  setupDisplay();
  setupUart();
  EnableInterrupts();
  AD1CON1bits.ASAM = 1;
  while (1)
  {
    printString("\rRB3: ");
    printChar('0' + PORTBbits.RB3);
    printString(" RB2: ");
    printChar('0' + PORTBbits.RB2);
    printString(" RB1: ");
    printChar('0' + PORTBbits.RB1);
    printString(" RB0: ");
    printChar('0' + PORTBbits.RB0);
    buffer[last] =
        PORTBbits.RB3 << 3 |
        PORTBbits.RB2 << 2 |
        PORTBbits.RB1 << 1 |
        PORTBbits.RB0;
    last = (last + 1) % 16;
    if (first == last)
      first = (first + 1) % 16;
    delay(1000 / freq);
  }
  return 0;
}

void _int_(24) isr_uart()
{
  if (IFS0bits.U1RXIF)
  {
    if (U1RXREG == 'E')
      printBuffer();
  }
  IFS0bits.U1RXIF = 0;
}

void _int_(27) isr_adc()
{
  freq = 1 + (ADC1BUF0 + 127) / 255;
  AD1CON1bits.ASAM = 1;
  IFS1bits.AD1IF = 0;
}

void _int_(8) isr_timer2()
{
  display(freq);
  IFS0bits.T2IF = 0;
}
