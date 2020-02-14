/*
   PIR sensor tester
*/
#define RE 587.33
#define MI 659.26
#define FA 739.99
#define SOL 783.99
#define LA 880.00
#define SI 987.77

int bottomPIRPin = 2;               // choose the input pin (for PIR sensor)
int topPIRPin = 3;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int bottomVal = LOW;                    // variable for reading the pin status
int topVal = LOW;
int pendingAlert = LOW;
unsigned long timei = 0;
int counter = 0;

const int buzzer = 9; //buzzer to arduino pin 9
const float tempo = 400;
float tempoDelay = 1000*(60/tempo);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);      // declare LED as output
  pinMode(bottomPIRPin, INPUT);     // declare sensor as input
  pinMode(topPIRPin, INPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

  Serial.begin(9600);
  Serial.println(tempo);
  Serial.println(tempoDelay);
}

void note(float freq, int duration) {
  tone(buzzer, freq, duration*tempoDelay);
  Serial.println(freq);
  Serial.println(duration);
  delay(duration*tempoDelay);
}

void loop() {
  bottomVal = digitalRead(bottomPIRPin);  // read input value
  topVal = digitalRead(topPIRPin);

  if (bottomVal && not(topVal) && not(pendingAlert)) { // trigger pending alert countdown 
    timei = millis();
    pendingAlert = HIGH;
    Serial.println("Suspicious activiy, triggering countdown..");
  }

  if (topVal && pendingAlert) { // reset pending alert
    pendingAlert = LOW;
    counter = 0;
    Serial.println("Reset pending alert.");
    delay(2000);
  }

  if (pendingAlert && (millis() - timei > 5000)) {
    Serial.println("Alert !");

    switch (counter++) {
      case 0:
        note(RE, 1);
        break;
      case 1:
        note(MI, 1);
        break;
      case 2:
        note(FA, 3);
        break;
      case 3:
        note(LA, 3);
        break;
      case 4:
        note(FA, 3);
        break;
      case 5:
        note(MI, 1);
        break;
      case 6:
        note(FA, 1);
        break;
      case 7:
        note(MI, 1);
        break;
      case 8:
        note(RE, 3);
        break;
      case 9:
        delay(6*tempoDelay);
        break;
    }

    counter = counter % 10;
    
    Serial.println(millis()-timei);
    //delay(1000);
  }


}
