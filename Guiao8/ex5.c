#include <detpic32.h>

#include "./../helper.c"

struct Config {
  unsigned int baudrate;
  char parity;
  unsigned int stop;
};

int main() {
  struct Config configs[5] = {{600, 'N', 1},
                              {1200, 'O', 2},
                              {9600, 'E', 1},
                              {19200, 'N', 2},
                              {115200, 'E', 1}};
  int testType = 0;
  while (1) {
    my_puts("String de teste\n");
    delay(1000);
  }
}