int pin = 5;
void setup() {
  pinMode(pin, OUTPUT);

}

void loop() {
  for (int i=0; i < 1000; ++i) {
    if (i%25==0) digitalWrite(pin, HIGH);
    else digitalWrite(pin, LOW);
  }
}
