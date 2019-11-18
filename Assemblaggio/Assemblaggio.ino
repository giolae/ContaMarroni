#include <HX711.h>

#include <ctype.h>
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifdef _Adafruit_SSD1306_H_
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // SDA A4 Giallo Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_SCL       5 // SCL A5 verde )

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif


int eepromAdress = 0;
#ifdef HX711_h
// Declaration for an HX711
const int bilancia_DOUT_PIN = 3;  // Blu
const int bilancia_SCK_PIN = 2;   // Bianco
float bilanciaCalibrationFactor = 387;
HX711 bilancia;
#endif

//  E18-D80NK Infrared Distance Ranging Sensor
#define IRD80NK_DIN_PIN  5  // Arancione

int marroniCounter = 0;     // numero di marroni pesati
int marroniGrammi = 0;      // totale del peso dei marroni nella bilancia
int marroniCorrente = 0;    // peso dell'ultimo marrone
int marroniPezzatura = 0;   // rapporto marroniGrammi / marroniCounter
bool marroniDetected = false;
int marroniDist[40]; // marroniDist[0] grammi 1 - marroniDist[39] grammi 40

char currentAction = NULL;

void setup() {
  memset(marroniDist, 0, sizeof(marroniDist));
  Serial.begin(9600);
  Serial.println("SetUp Begin");

#ifdef _Adafruit_SSD1306_H_

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(500); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel bilancia
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
#endif

#ifdef HX711_h
  bilancia.begin(bilancia_DOUT_PIN , bilancia_SCK_PIN );
  delay(250);

  bilancia.set_scale(bilanciaCalibrationFactor ); //Adjust to this calibration factor
  delay(250);

  bilancia.tare(); //Reset the bilancia to 0
  delay(250);

  long zero_factor = bilancia.read_average(); //Get a baseline reading
  delay(250);
  Serial.print(F("Zero factor: ")); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
#endif

  Serial.println(F("SetUp End"));
}

void loop() {
  //  char   _input = NULL;

  if (Serial.available() > 0) {
    char     _input = Serial.read();     //read the input
    readCommand(_input);
  }

  //  String _t =  timeToString();
  //  ssd1306Println(_t + " EG " + currentAction);

  //  delay(2000);
}

void readCommand(char _input) {
  switch (_input) {
    case 's':
    case 'S':
      actionStart(_input);
      break;

    case 'e':
    case 'E':
      actionEnd(_input);
      break;

    case 'c':
    case 'C':
      actionCalibrateHX711(_input);
      break;

    case 't':
    case 'T':
      actionTare(_input);
      break;

    case 'r':
    case 'R':
      actionReadWeight(_input);
      break;

    default:
      break;
  }

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

#ifdef _Adafruit_SSD1306_H_

  display.setCursor(0, 0);     // Start at top-left corner
  display.clearDisplay();
  display.println(aText);
  display.display();
#endif
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
void actionCalibrateHX711(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'C';
  doCalibrateHX711();
}
void actionTare(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'T';
  doTare();
}
void actionReadWeight(char aStato)
{
  if (!verificaCambioStato(aStato)) return;
  currentAction = 'R';
  doReadWeight();
}

boolean verificaCambioStato(char aStato) {
  aStato = toupper(aStato);
  Serial.print(F("richiesta Azione: "));
  Serial.println(aStato);
  if ((currentAction == NULL) && (aStato == 'S')) return true;
  if ((currentAction == NULL) && (aStato == 'C')) return true;
  if ((currentAction == NULL) && (aStato == 'T')) return true;
  if ((currentAction == NULL) && (aStato == 'R')) return true;

  if ((currentAction == 'S') && (aStato == 'E')) return true;

  Serial.print(F("Azione: "));
  Serial.print(aStato);
  Serial.print(F(" non compatibile con lo stato corrente: ") );
  Serial.println(currentAction);
  return false;
}
