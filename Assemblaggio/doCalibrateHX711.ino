void doCalibrateHX711() {
  Serial.println(F("doCalibrateHX711 START"));
  CalibrateHX711();
  Serial.println(F("doCalibrateHX711END"));
  currentAction = NULL;
}

float doReadWeight() {
  currentAction = NULL;
  return readWeight();
}
float readWeight() {
  float _grammi ;
#if defined(HX711_h)
  // returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
  // times = how many readings to do
  // float get_units(byte times = 1);

 Serial.print(F("start Reading: "));
 
  _grammi = bilancia.get_units(5);

  Serial.print(F("Reading: "));
  Serial.print(_grammi, 2);
  Serial.print(" gr."); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(F(" bilanciaCalibrationFactor : "));
  Serial.print(bilanciaCalibrationFactor );
  Serial.println();

#endif
  return _grammi;
}

void CalibrateHX711() {
#if defined(HX711_h)

  Serial.println(F("Remove all weight from scale"));
  Serial.println(F("After readings begin, place known weight on scale"));
  Serial.println(F("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively"));
  Serial.println(F("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively"));
  Serial.println(F("Press t for tare"));
  Serial.println(F("Press e for exit"));
  bilancia.set_scale();
  bilancia.tare(); //Reset the scale to 0

  long zero_factor = bilancia.read_average(); //Get a baseline reading
  Serial.print(F("Zero factor: ")); //This can be used to remove the need to tare the bilancia. Useful in permanent scale projects.
  Serial.println(zero_factor);

  bilancia.set_scale(bilanciaCalibrationFactor ); //Adjust to this calibration factor

  char input ;
  do {
    if (Serial.available())
    {
      input = Serial.read();
      if (input == '-' )
        bilanciaCalibrationFactor += 1;
      else if (input == 'a')
        bilanciaCalibrationFactor += 10;
      else if (input == '+' )
        bilanciaCalibrationFactor -= 1;
      else if ( input == 'z')
        bilanciaCalibrationFactor -= 10;
      else if (input == 's')
        bilanciaCalibrationFactor += 100;
      else if (input == 'x')
        bilanciaCalibrationFactor -= 100;
      else if (input == 'd')
        bilanciaCalibrationFactor += 1000;
      else if (input == 'c')
        bilanciaCalibrationFactor -= 1000;
      else if (input == 'f')
        bilanciaCalibrationFactor += 10000;
      else if (input == 'v')
        bilanciaCalibrationFactor -= 10000;
      else if (input == 't')
        doTare();  //Reset the scale to zero

    }


    bilancia.set_scale(bilanciaCalibrationFactor ); //Adjust to this calibration factor

    //    Serial.print("Reading: ");
    //    Serial.print(bilancia.get_units(), 2);
    //    Serial.print(" gr."); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    //    Serial.print(" bilanciaCalibrationFactor : ");
    //    Serial.print(bilanciaCalibrationFactor );
    //    Serial.println();

    readWeight();
  } while (input != 'e');

#endif

}

void isHX711Ready() {
#if defined(HX711_h)
  if (bilancia.wait_ready_timeout(1000)) {
    long reading = bilancia.read();
    Serial.print(F("HX711 reading: "));
    Serial.println(reading);
  } else {
    Serial.println(F("HX711 not found."));
  }
#endif
}
