#include <HX711_ADC.h>

//#include <HX711.h>

#include <ctype.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declaration for an HX711
const int bilancia_DOUT_PIN = 2;  // Blu
const int bilancia_SCK_PIN = 3;   // Bianco
const float GAIN_FACTOR = 0.002155172;
int eepromAdress = 0;
HX711_ADC bilancia(bilancia_DOUT_PIN , bilancia_SCK_PIN);

bool item_detected = false;
int  item_counter = 0;
char currentAction = NULL;

void setup() {
  Serial.begin(9600);
  Serial.println("SetUp Begin");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel bilancia
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  bilancia.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  bilancia.start(stabilisingtime);
  if (bilancia.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    bilancia.setCalFactor(1.0); // user set calibration value (float)
    Serial.println("Startup + tare is complete");
  }
  Serial.println("SetUp End");
}
void loop1()
{
  char   input = Serial.read();     //read the input
  if (input == 's' || input == 'S') {
    actionStart(input);
  }
  if (input == 'e' || input == 'E') {
    actionEnd(input);
  }
  if (input == 'c' || input == 'C') {
    actionCalibrate(input);
  }
  if (input == 't' || input == 'T') {
    actionTare(input);
  }

}

void loop() {
  char   input = Serial.read();     //read the input
  if (input == 's' || input == 'S') {
    actionStart(input);
  }
  if (input == 'e' || input == 'E') {
    actionEnd(input);
  }
  if (input == 'c' || input == 'C') {
    actionCalibrate(input);
  }
  if (input == 't' || input == 'T') {
    actionTare(input);
  }

  //  int val = digitalRead( COUNTER_PIN );
  //  if ( (item_detected == false) && ( val == LOW )) {
  //    item_detected = true;
  //    digitalWrite(LED_BUILTIN, HIGH);
  //    item_counter++;
  //    updateCounter();
  //    readWeight();
  //  }
  //
  //  else if ( (item_detected == true) && ( val == HIGH )) {
  //    item_detected = false;
  //    digitalWrite(LED_BUILTIN, LOW);
  //  }

  // Clear the buffer
  display.clearDisplay();
  String _t =  timeToString();
  ssd1306Println(_t + " EG " + currentAction);

  delay(2000);
}

String timeToString() {
  char _time[10] = "";
  timeToString(_time);
  return String(_time);
}

void timeToString(char* aString) {

  //  timeToString(str, sizeof(str))
  unsigned long nowMillis = millis();
  unsigned long seconds = nowMillis / 1000;
  seconds %= 86400;
  byte hours = seconds / 3600;
  seconds %= 3600;
  byte minutes = seconds / 60;
  seconds %= 60;
  sprintf(aString,  "%02d:%02d:%02d",  hours, minutes, seconds);
}
void ssd1306Println(String aText) {
  //  int _l = aText.length();
  //  byte _b[_l];
  //  aText.getBytes(_b, _l);
  //  ssd1306Println(_b);
  Serial.println(aText);

  display.setCursor(0, 0);     // Start at top-left corner
  display.clearDisplay();
  display.println(aText);
  display.display();
}

void actionStart(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'S';
  doStart();
}
void actionEnd(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'E';
  doEnd();
}
void actionCalibrate(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'C';
  doCalibrate();
}
void actionTare(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'T';
  doTare();
}

boolean verificaCambioStato(char aStato) {
  aStato = toupper(aStato);
  Serial.print("richiesta Azione: ");
  Serial.println(aStato);
  if ((currentAction == NULL) && (aStato == 'S')) return true;
  if ((currentAction == NULL) && (aStato == 'C')) return true;
  if ((currentAction == NULL) && (aStato == 'T')) return true;

  if ((currentAction == 'S') && (aStato == 'E')) return true;

  Serial.print("Azione: ");
  Serial.print(aStato);
  Serial.print(" non compatibile con lo stato corrente: " );
  Serial.println(currentAction);
  return false;
}
void readWeight() {
  delay(100);
}

void updateCounter() {
  Serial.println("Item count; " + item_counter);
}


void doStart() {
  Serial.println("doStart START");
  delay(500);
  Serial.println("doStart END");
}


void doEnd() {
  Serial.println("doEnd START");
  delay(500);
  Serial.println("doEnd END");
  currentAction = NULL;
}

void doTare() {
  Serial.println("doTare START");


  //receive from serial terminal
  bilancia.tareNoDelay();
  boolean f = 0;
  do {
    delay(100);

  } while (bilancia.getTareStatus() == false);


  Serial.println("Tare complete");
  Serial.println("doTare END");
  currentAction = NULL;
}

void doCalibrate() {
  Serial.println("doCalibrate START");
  delay(500);
  Serial.println("doCalibrateEND");
  currentAction = NULL;
}
