#include <detpic32.h>
#include "./../helper.c"


int main() {
  configUART1(115200, 'N', 8, 1);
  while (1) {
    my_puts("String de teste\n");
    delay(1000);
  }
}