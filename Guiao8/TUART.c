#include <detpic32.h>

#include "./../helper.c"

void delay(int ms) {
  for (; ms > 0; ms--) {
    resetCoreTimer();
    while (readCoreTimer() < 20000)
      ;
  }
}

void _int_(12) isr_timer3(void) {
  U1TXREG = 'U';
  IFS0bits.T3IF = 0;
}

int main(void) {
  int bit_time, baudrate;
  delay(1000);
  T3CONbits.TCKPS = 7;
  PR3 = 39064;
  TMR3 = 0;
  T3CONbits.TON = 1;
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;

  baudrate = 4800;
  // baudrate = 57600;
  configUART1(baudrate, 'N', 8, 1);
  while (1) {
    EnableInterrupts();
    while (PORTDbits.RD3 == 1)
      ;  // espera que deixe de ser 1 para sincronizar
    while (PORTDbits.RD3 == 0)
      ;  // espera pela transição de 0 --> 1
    resetCoreTimer();
    while (PORTDbits.RD3 == 1)
      ;                               // espera pela transição de 1 --> 0
    bit_time = readCoreTimer() * 50;  // ns
    DisableInterrupts();
    printf("Tempo de bit (ns): %d\n", bit_time);
    printf("Baudrate: %d\n", 1000000000 / bit_time);
    my_getc();
  }
}
