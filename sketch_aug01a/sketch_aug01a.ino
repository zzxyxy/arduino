const int sensorPin = A0;    // select the input pin for the potentiometer
const int led = 9;      // select the pin for the LED
const int controlled = 13;      // select the pin for the LED
const int movedet = 8;
int sensor_value = 0;  // variable to store the value coming from the sensor
int moveing = 0;
#define RESET_COUNTER 200

int counter = RESET_COUNTER;
int m;
#define MAX 5

bool dark = false;
bool ledon = false;
int ledactual = 0;

void adjustled() 
{
  if (ledon) {
    if (ledactual < MAX)
      ledactual++;
    else
      return;    
  } else {
    if (ledactual > 0)
      ledactual--;
    else
      return;        
  }
  analogWrite(led, ledactual);
}

void setup() {
//  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(controlled, OUTPUT);
  pinMode(movedet, INPUT);
  // declare the ledPin as an OUTPUT:
//  analogWrite(led,MAX);
}

void loop() {
  sensor_value = analogRead(sensorPin);
  if (sensor_value < 20) {
    dark = true;
  } else {
    dark = false;
  }

//  Serial.println(ledactual);
  m = digitalRead(movedet);
  
  if (m) {
    moveing = true;
    counter = RESET_COUNTER;
  } else {
    if (counter > 0) {
      --counter;
    } else {
        moveing = false;
    }
  }

  if (ledon)
    digitalWrite(controlled, HIGH);
  else
    digitalWrite(controlled, LOW);
    
  if (moveing) {
    if (ledactual > 0) {
      ledon = true;
    }
    if (ledactual == 0 && dark) {
      ledon = true;
    }
  } else {
    ledon = false;
  }

  adjustled();
  delay(50);  
}
