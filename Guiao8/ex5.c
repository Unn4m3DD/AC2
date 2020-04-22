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
  struct Config config = configs[testType];
  configUART1(config.baudrate, config.parity, 8, config.stop);
  while (1) {
    my_puts("String de teste\n");
    delay(1000);
  }
}