/*
  E18-D80NK Infrared Distance Ranging Sensor
*/
#define IRD80NK_DIN_PIN = 5;  // Arancione

int marroniCount = 0;
int marroneDetected = false;
void setup() {
  Serial.begin(9600); //Start serial communication boud rate at 9600
  pinMode(5, INPUT); //Pin 5 as signal input
}
void loop() {
  while (1) {
    delay(500);
    if (digitalRead(IRD80NK_DIN_PIN ) == LOW) {
      // If no signal print collision detected
      Serial.println("Collision Detected.");
    }
    else {
      // If signal detected print collision detected
      Serial.println("No Collision Detected.");
    }
  }
}


int val = digitalRead( IRD80NK_DIN_PIN );
if ( (marroneDetected == false) && ( val == LOW )) {
  marroneDetected = true;
  marroniCounter++;
  updateCounter();
}

else if ( (marroneDetected == true) && ( val == HIGH )) {
  marroneDetected = false;
}
