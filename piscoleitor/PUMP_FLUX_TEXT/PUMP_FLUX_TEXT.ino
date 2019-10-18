static const int PUMP = 4;
static const int PUMP2 = 2;
void setup() {
  // put your setup code here, to run once:
 pinMode(PUMP,OUTPUT);
 pinMode(PUMP2,OUTPUT);
}

void loop() {
  digitalWrite(PUMP,255);
  delay(5000);
  digitalWrite(PUMP,0);
  delay(5000);
  digitalWrite(PUMP2,255);
}
