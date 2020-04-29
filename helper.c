#include <detpic32.h>
#pragma once
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1
int codes[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void send2displaysHex(unsigned char value) {
  static int current_display = 0;
  if (current_display) {
    int num1 = (value) % 16;
    LATB = codes[num1] << 8;
    LATD ^= 0x0060;
  } else {
    int num2 = (value >> 4) % 16;
    LATB = codes[num2] << 8;
    LATD ^= 0x0060;
  }
  current_display = current_display ? 0 : 1;
}

void send2displaysDec(unsigned char value) {
  static int current_display = 0;
  int dot = 0;
  if (current_display) {
    int num1 = (value) % 10;
    LATB = codes[num1] << 8;
    LATBbits.LATB15 = dot;
    LATD ^= 0x0060;
  } else {
    int num2 = (value / 10) % 10;
    LATB = codes[num2] << 8;
    LATBbits.LATB15 = dot ? 0 : 1;
    LATD ^= 0x0060;
  }
  current_display = current_display ? 0 : 1;
}
void delay(unsigned int ms) {
  int k = PBCLK / 1000;
  for (; ms > 0; ms--) {
    resetCoreTimer();
    while (readCoreTimer() < k)
      ;
  }
}
void setPWM1(unsigned int dutyCycle, unsigned int PRxTimer) {
  OC1RS = PRxTimer * dutyCycle / 100;
}

void configUART1(unsigned int baudrate, char parity, unsigned int bit_count,
                 unsigned int stop_bits_count) {
  baudrate = 600 <= baudrate && baudrate <= 115200 ? baudrate : 115200;
  parity = parity == 'N' || parity == 'O' || parity == 'O' ? parity : 'N';
  stop_bits_count =
      stop_bits_count == 1 || stop_bits_count == 2 ? stop_bits_count : 1;
  bit_count = bit_count == 8 || bit_count == 9 ? bit_count : 8;

  U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
  U1MODEbits.BRGH = 0;
  U1MODEbits.PDSEL =
      bit_count == 9 ? 3 : parity == 'N' ? 0 : parity == 'E' ? 1 : 2;
  U1MODEbits.STSEL = stop_bits_count == 1 ? 0 : 1;
  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;
  U1MODEbits.ON = 1;
}

void my_putc(char byte_to_send) {
  while (U1STAbits.UTXBF == 1)
    ;
  U1TXREG = byte_to_send;
}

void my_puts(char* str) {
  char c;
  for (c = *str; *str != '\0'; str++) {
    my_putc(*str);
  }
}

char my_getc(void) {
  if (U1STAbits.OERR == 1) U1STAbits.OERR = 0;
  while (U1STAbits.URXDA == 0)
    ;
  if (U1STAbits.FERR || U1STAbits.PERR) {
    int garb = U1RXREG;
    return 0;
  } else {
    return U1RXREG;
  }
}
