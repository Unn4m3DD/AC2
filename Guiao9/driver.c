#include <detpic32.h>

#include "./../helper.c"
#define BUF_SIZE 8
#define INDEX_MASK BUF_SIZE - 1
typedef struct {
  unsigned char data[BUF_SIZE];
  unsigned int first;
  unsigned int last;
  unsigned int count;
} circularBuffer;

volatile circularBuffer rxb, txb;

void comDrv_flushRx(void) {
  rxb.first = 0;
  rxb.last = 0;
  rxb.count = 0;
}

void comDrv_flushTx(void) {
  txb.first = 0;
  txb.last = 0;
  txb.count = 0;
}

// [_, 1, 2, 3, _, _]
//    ^        ^
//    first    last
void comDrv_putc(char ch) {
  while (txb.count == BUF_SIZE) {
  }
  txb.data[txb.last] = ch;
  txb.last = ((txb.last + 1) & INDEX_MASK);
  DisableUart1TxInterrupt();
  txb.count++;
  EnableUart1TxInterrupt();
}

void comDrv_puts(char* str) {
  char c;
  for (c = *str; *str != '\0'; str++) {
    my_putc(*str);
  }
}
void _int_(24) isr_uart1(void) {
  if (txb.count != 0) {
    U1TXREG = txb.data[txb.first];
    txb.first = ((txb.first + 1) & INDEX_MASK);
    txb.count--;
  }
  if (txb.count == 0) DisableUart1TxInterrupt();
  IFS0bits.U1TXIF = 0;
}
// [_, 1, 2, 3, _, _]
//    ^        ^
//    first    last
/*
char comDrv_getc(char* pchar) {
  if (rxb.count == 0) return 0;
  DisableUart1RxInterrupt();  // Begin of critical section
  *pchar = rxb.first;
  rxb.first = (rxb.first + 1) & INDEX_MASK;
  rxb.count--;
  EnableUart1RxInterrupt();  // End of critical section
  return 1;
}
*/
int main() {
  configUART1(115200, 'N', 8, 1);
  IPC6bits.U1IP = 3;
  comDrv_flushRx();
  comDrv_flushTx();
  EnableInterrupts();
  while (1) comDrv_puts("1234\n");
}
