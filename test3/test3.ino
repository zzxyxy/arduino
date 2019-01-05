

int ledPin = 13;

int knockSensor = 4;               
byte val = 0;
int statePin = LOW;
int THRESHOLD = 100;



void setup() {
 pinMode(ledPin, OUTPUT); 
 Serial.begin(9600);
}


void loop() {
  val = analogRead(knockSensor);     
  Serial.print("val: ");
  Serial.println(val);

  if (val >= THRESHOLD) {
    statePin = !statePin;
    digitalWrite(ledPin, statePin);
    Serial.println("Knock!");
  }

  delay(100);  // we have to make a delay to avoid overloading the serial port

}
