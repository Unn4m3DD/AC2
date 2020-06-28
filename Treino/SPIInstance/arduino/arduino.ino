#define CLK 16 //D0
#define IN 5   //D1
#define OUT 4  //D2
void setup() {
  Serial.begin(9600);
  pinMode(CLK, INPUT);
  pinMode(IN, INPUT);
  pinMode(OUT, OUTPUT);
  Serial.print("0");
}

void loop() {
  while (digitalRead(CLK) == 0) ;
  char c = 0;
  for (int i = 0; i < 8; i++) {
    while (digitalRead(CLK) == 1);
    c |= digitalRead(IN) << i;
    while (digitalRead(CLK) == 0);
  }
  Serial.printf("current: %c\n", c);
}
