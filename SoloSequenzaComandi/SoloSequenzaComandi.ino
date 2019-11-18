#include <ctype.h>



// IR DN80NK circuit wiring
//#define COUNTER_PIN 7

// HX711 circuit wiring
const int bilancia_DOUT_PIN = 2;  // Blu
const int bilancia_SCK_PIN = 3;   // Bianco
const float GAIN_FACTOR = 0.002155172;
int eepromAdress = 0;
//HX711_ADC bilancia(bilancia_DOUT_PIN , bilancia_SCK_PIN);
//
bool item_detected = false;
int  item_counter = 0;
char currentAction = NULL;
void setup()
{
  Serial.begin(9600);
  Serial.println("SetUP Init");
  Serial.println("SetUP Done");

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
void loop()
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
  currentAction=NULL;
}

void doTare() {
  Serial.println("doTare START");
  delay(500);
  Serial.println("doTare END");
  currentAction=NULL;
}

void doCalibrate() {
  Serial.println("doCalibrare START");
  delay(500);
  Serial.println("***");
  Serial.println("***");
  Serial.println("Start calibration:");
  Serial.println("It is assumed that the mcu was started with no load applied to the load cell.");
  Serial.println("Now, place your known mass on the bilancia,");
  Serial.println("then send the weight of this mass (i.e. 100.0) from serial monitor.");
//  float m = 0;
//  boolean f = 0;
//  while (f == 0) {
//    bilancia.update();
//    if (Serial.available() > 0) {
//      m = Serial.parseFloat();
//      if (m != 0) {
//        Serial.print("Known mass is: ");
//        Serial.println(m);
//        f = 1;
//      }
//      else {
//        Serial.println("Invalid value");
//      }
//    }
//  }
//  float c = bilancia.getData() / m;
//  bilancia.setCalFactor(c);
//  Serial.print("Calculated calibration value is: ");
//  Serial.print(c);
//  Serial.println(", use this in your project sketch");
//  f = 0;
//  Serial.print("Save this value to EEPROM adress ");
//  Serial.print(eepromAdress);
//  Serial.println("? y/n");
//  while (f == 0) {
//    if (Serial.available() > 0) {
//      char inByte = Serial.read();
//      if (inByte == 'y') {
////#if defined(ESP8266)
////        EEPROM.begin(512);
////#endif
////        EEPROM.put(eepromAdress, c);
////#if defined(ESP8266)
////        EEPROM.commit();
////#endif
////        EEPROM.get(eepromAdress, c);
////        Serial.print("Value ");
////        Serial.print(c);
////        Serial.print(" saved to EEPROM address: ");
////        Serial.println(eepromAdress);
////        f = 1;
////
//      }
//      else if (inByte == 'n') {
//        Serial.println("Value not saved to EEPROM");
//        f = 1;
//      }
//    }
//  }
  Serial.println("End calibration");
  Serial.println("For manual edit, send 'c' from serial monitor");
  Serial.println("***");
  Serial.println("doCalibrare END");
  currentAction=NULL;
}

//void doCalibrate1() {
////  Serial.println("HX711 Calibration");
////  Serial.println("Remove all weight from scale");
////  Serial.println("After readings begin, place known weight on scale");
////  Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
////  Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");
////  Serial.println("Press t for tare");
////  Serial.println("Press e for exit");
////  bilancia.set_scale();
////  bilancia.tare(); //Reset the scale to 0
////
////  long zero_factor = bilancia.read_average(); //Get a baseline reading
////  Serial.print("Zero factor: "); //This can be used to remove the need to tare the bilancia. Useful in permanent scale projects.
////  Serial.println(zero_factor);
////
////  bilancia.set_scale(calibration_factor); //Adjust to this calibration factor
////
////  do {
////    if (Serial.available())
////    {
////      char input = Serial.read();
////      if (input == '+' || input == 'a')
////        calibration_factor += 10;
////      else if (input == '-' || input == 'z')
////        calibration_factor -= 10;
////      else if (input == 's')
////        calibration_factor += 100;
////      else if (input == 'x')
////        calibration_factor -= 100;
////      else if (input == 'd')
////        calibration_factor += 1000;
////      else if (input == 'c')
////        calibration_factor -= 1000;
////      else if (input == 'f')
////        calibration_factor += 10000;
////      else if (input == 'v')
////        calibration_factor -= 10000;
////      else if (input == 't')
////        bilancia.tare();  //Reset the scale to zero
////      if (input == 'e' || input == 'E')
////    }
////
////    bilancia.set_scale(calibration_factor); //Adjust to this calibration factor
////
////    Serial.print("Reading: ");
////    Serial.print(bilancia.get_units(), 3);
////    Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
////    Serial.print(" calibration_factor: ");
////    Serial.print(calibration_factor);
////    Serial.println();
////
////  } while (input != 'e')
//
//}
