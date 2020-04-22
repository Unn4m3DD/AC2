#include <detpic32.h>

#include "./../helper.c"

void UART1Interrupts(int priority) {
  U1STAbits.URXISEL0 = 0;
  U1STAbits.URXISEL1 = 0;
  IEC0bits.U1RXIE = 1;
  IPC6bits.U1AIP = 3;
}

int main() {
  configUART1(115200, 'N', 8, 1);
  UART1Interrupts(3);
  EnableInterrupts();
  while (1)
    ;
}

void _int_(24) isr_uart1(void){
  my_putc(U1RXREG); 
  IFS0bits.U1RXIF = 0;
}
