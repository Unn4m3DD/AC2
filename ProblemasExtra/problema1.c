#include <detpic32.h>

int display_codes[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                         0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

volatile white_time = 400, black_time = 400;
volatile current_white = 0;
void configLed() {
  TRISEbits.TRISE0 = 0;
  TRISEbits.TRISE1 = 0;
  TRISEbits.TRISE2 = 0;
  TRISEbits.TRISE3 = 0;
}

void configDisplay() {
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  TRISBbits.TRISB14 = 0;
  TRISBbits.TRISB13 = 0;
  TRISBbits.TRISB12 = 0;
  TRISBbits.TRISB11 = 0;
  TRISBbits.TRISB10 = 0;
  TRISBbits.TRISB9 = 0;
  TRISBbits.TRISB8 = 0;
}

void configDisplayTimer() {  //160hz
  T2CONbits.TCKPS = 1;
  PR2 = 62499;
  TMR2 = 0;
  IPC2bits.T2IP = 2;
  IEC0bits.T2IE = 1;
  IFS0bits.T2IF = 0;
  T2CONbits.TON = 1;
}

void sentToDisplays(char left, char right) {
  static current_display = 0;
  if (current_display == 0) {
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    LATB = (display_codes[left] << 8);
  } else {
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    LATB = (display_codes[right] << 8);
  }
  current_white = current_white == 0 ? 1 : 0;
}

void showToUser() {
  int time_to_show = current_white == 1 ? white_time : black_time;
  int led_amount = time_to_show / 100 % 10;
  if (led_amount <= 1)
    LATEbits.LATE4 = 1;
  if (led_amount <= 2)
    LATEbits.LATE5 = 1;
  if (led_amount <= 3)
    LATEbits.LATE6 = 1;
  if (led_amount <= 4)
    LATEbits.LATE7 = 1;
  sentToDisplays(time_to_show / 10 % 10, time_to_show % 10);
}

int main() {
  configLed();
  configDisplay();
  configTimer1();
  while (white_time >= 0 && black_time >= 0) {
    char current_char = getChar();
    if (current_char == 'b') {
      current_white = current_white == 0 ? 1 : 0;
    }
    if (current_char == 'x') {
      break;
    }
  }
  printf("%s ganhou", current_white == 1 ? "branco" : "preto");

  return 0;
}

void _int_(8) isr_T2() {
  showToUser();
}