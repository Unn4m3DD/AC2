#include <detpic32.h>
#include <stdlib.h>

volatile int val = 0, min_val = 99999, max_val = 0;

void setPWM1(unsigned int dutyCycle, unsigned int PRxTimer)
{
  OC1RS = PRxTimer * dutyCycle / 100;
}

void configUART2(unsigned int baudrate, char parity, unsigned int bit_count,
                 unsigned int stop_bits_count)
{
  baudrate = 600 <= baudrate && baudrate <= 115200 ? baudrate : 115200;
  parity = parity == 'N' || parity == 'O' || parity == 'O' ? parity : 'N';
  stop_bits_count =
      stop_bits_count == 1 || stop_bits_count == 2 ? stop_bits_count : 1;
  bit_count = bit_count == 8 || bit_count == 9 ? bit_count : 8;

  U2BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
  U2MODEbits.BRGH = 0;
  U2MODEbits.PDSEL =
      bit_count == 9 ? 3 : parity == 'N' ? 0 : parity == 'E' ? 1 : 2;
  U2MODEbits.STSEL = stop_bits_count == 1 ? 0 : 1;
  U2STAbits.UTXEN = 1;
  U2STAbits.URXEN = 1;
  U2MODEbits.ON = 1;
}

void my_putc(char byte_to_send)
{
  while (U2STAbits.UTXBF == 1)
    ;
  U2TXREG = byte_to_send;
}

void my_puts(char *str)
{
  char c;
  for (c = *str; *str != '\0'; str++)
  {
    my_putc(*str);
  }
}
void send2displaysDec(unsigned char value)
{
  int codes[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                   0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static int current_display = 0;
  int dot = 0;
  if (current_display)
  {
    int num1 = (value) % 10;
    LATB = codes[num1] << 8;
    LATBbits.LATB15 = dot;
    LATD ^= 0x0060;
  }
  else
  {
    int num2 = (value / 10) % 10;
    LATB = codes[num2] << 8;
    LATBbits.LATB15 = dot ? 0 : 1;
    LATD ^= 0x0060;
  }
  current_display = current_display ? 0 : 1;
}
void configT1()
{
  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;
  T1CONbits.TCKPS = 3;
  PR1 = 78121 / 4;
  TMR1 = 0;
  T1CONbits.TON = 1;
}

void configT3()
{
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;
  T3CONbits.TCKPS = 5;
  PR3 = 6249;
  TMR3 = 0;
  T3CONbits.TON = 1;
}

void configADC()
{
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 1;
  AD1CON2bits.SMPI = 4 - 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;
  IPC6bits.AD1IP = 2;
  IFS1bits.AD1IF = 0;
  IEC1bits.AD1IE = 1;
}

void configDisplay()
{
  TRISB &= 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
}

void configSwitches()
{
  TRISBbits.TRISB0 = 1;
  TRISBbits.TRISB1 = 1;
}

void configPWM()
{
  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 1;
  OC1CONbits.ON = 1;
}
void routine00()
{
  IEC0bits.T1IE = 1;
  LATEbits.LATE0 = 0;
}
void routine01()
{
  IEC0bits.T1IE = 0;
  LATEbits.LATE0 = 1;
}
void routine1x()
{
  IEC0bits.T1IE = 1;
  setPWM1(val * 3, 6249);
  LATEbits.LATE0 = LATDbits.LATD0;
}

void UART2Interrupts(int priority)
{
  U2STAbits.URXISEL0 = 0;
  U2STAbits.URXISEL1 = 0;
  IEC1bits.U2RXIE = 1;
  IPC8bits.U2IP = 3;
}

int main()
{
  configT1();
  configT3();
  configADC();
  configDisplay();
  configSwitches();
  configPWM();
  configUART2(115200, 'N', 8, 1);
  UART2Interrupts(3);
  AD1CON1bits.ASAM = 1;

  EnableInterrupts();
  void (*routines[])(void) = {&routine1x, &routine1x, &routine01, &routine00};
  while (1)
    routines[(int)(PORTB & 0x3)]();
}
void _int_(32) isr_uart2(void)
{
  if (IFS1bits.U2RXIF == 1)
  {
    if (U2RXREG == 'l' || U2RXREG == 'L')
    {
      my_puts("MAX: ");
      my_putc(max_val / 10 + 48);
      my_putc('.');
      my_putc(max_val % 10 + 48);
      my_puts("\nMIN: ");
      my_putc(min_val / 10 + 48);
      my_putc('.');
      my_putc(min_val % 10 + 48);
      my_putc('\n');
    }
    IFS1bits.U2RXIF = 0;
  }
}
void _int_(4) isr_T1(void)
{
  AD1CON1bits.ASAM = 1;
  IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
  static int counter = 0;
  counter++;
  if (counter > 100)
  {
    my_putc(val / 10 + 48);
    my_putc('.');
    my_putc(val % 10 + 48);
    my_putc('\n');
    counter = 0;
  }
  send2displaysDec(val);
  IFS0bits.T3IF = 0;
}

void _int_(27) isr_adc()
{
  int *p = (int *)(&ADC1BUF0);
  int i = 0;
  int media = 0;
  for (i = 0; i < 4; i++)
  {
    media += (p[i * 4] * 33 + 511) / 1023;
  }
  media /= 4;
  IFS1bits.AD1IF = 0;
  LATEbits.LATE0 = 0;
  val = media;
  if (min_val > media)
    min_val = media;
  if (max_val < media)
    max_val = media;
  IFS1bits.AD1IF = 0;
}
