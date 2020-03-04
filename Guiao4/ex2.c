#include <detpic32.h>
#include "./../helper.c"

int main(){
  TRISEbits.TRISE0 = 0;
  TRISEbits.TRISE1 = 0;
  TRISEbits.TRISE2 = 0;
  TRISEbits.TRISE3 = 0;
  int counter = 0;
  while(1){
    delay(250);
    LATE = (LATE & 0xfff0) ^ counter; 
    counter++;
  }
}