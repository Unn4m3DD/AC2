#include <detpic32.h>


void setupTimer(){
  T2CONbits.TCKPS = 2; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
  PR2 = 50000; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
  TMR2 = 0; // Reset timer T2 count register
  T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the
  IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
  IEC0bits.T2IE = 1; // Enable timer T2 interrupts
  IFS0bits.T2IF = 0;
}

void setupAdc(){
  TRISBbits.TRISB4 = 1; // RBx digital output disconnected
  AD1PCFGbits.PCFG4= 0; // RBx configured as analog input (AN4)
  AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
  AD1CON2bits.SMPI = 1-1; // Interrupt is generated after XX samples
  AD1CHSbits.CH0SA = 4; // replace x by the desired input
  IPC6bits.AD1IP = 2; // configure priority of A/D interrupts to 2 
  IEC1bits.AD1IE = 1; // enable A/D interrupts 
  IFS1bits.AD1IF = 0; // clear A/D interrupt flag 
  AD1CON1bits.ON = 1; // Enable A/D converter
}

volatile int timer = 0, last = 0, lastAdc = 0, configTimer = 0;
volatile unsigned char config = 0;

int main(){
  setupTimer();  
  setupAdc();
  EnableInterrupts();
  AD1CON1bits.ASAM = 1;  
  while(1){
    printf("\r%02d:%02d:%02d", timer / 6000 % 100, timer / 100 % 60, timer % 100);
  } 
 
  return 0;
}

void _int_(27) adc_isr(){
  if(ADC1BUF0 < last - 102 || last + 102 < ADC1BUF0){
    configTimer = 1000; 
    config = 1;
    last = ADC1BUF0;
  }
  if(config)
    timer = 6000 * ADC1BUF0 * 100 / 1023 / 6000 * 6000; 
  lastAdc = ADC1BUF0;
  AD1CON1bits.ASAM = 1;
  IFS1bits.AD1IF = 0;
}

void _int_(8) timer_isr(){
  if(configTimer < 0){
     configTimer = 0;
     config = 0;
     last = lastAdc;
  }
  if(config)
    configTimer--;
  else if (timer < 6000 * 99) 
    timer++;
  
  IFS0bits.T2IF = 0;
}






