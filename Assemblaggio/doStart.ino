void doStart() {
  Serial.println(F("doStart START"));

  marroniCounter = 0;
  marroniGrammi = 0;
  marroniPezzatura = 0;
  marroniDetected = false;
  marroniCorrente = 0;    // peso dell'ultimo marrone

  memset(marroniDist, 0, sizeof(marroniDist));

  displaySetLayout1() ;

  doTare();

  char     _input = NULL;
  do {
    if (Serial.available())
    {
      _input = Serial.read();     //read the input
      readCommand(_input);
    }
    int val = digitalRead( IRD80NK_DIN_PIN );
    if ( (marroniDetected == false) && ( val == LOW )) {
      digitalWrite(LED_BUILTIN, HIGH);
      marroniDetected = true;
    } else if ( (marroniDetected == true) && ( val == HIGH )) {
      digitalWrite(LED_BUILTIN, LOW);
      marroniDetected = false;
      marroniCounter++;
      delay(100);
      float _grammi = doReadWeight();

      marroniCorrente = max(1, (int)_grammi - marroniGrammi);
      marroniCorrente = min(marroniCorrente, 39);
      marroniDist[marroniCorrente]++;

      marroniGrammi = _grammi;

      marroniPezzatura = marroniCounter * 1000 / marroniGrammi ;
      displayUpdateLayout1();
      //    drawChart();
    }
  } while (_input != 'e');

  doReadWeight();
  printDist();
  Serial.println(F("doStart END"));
}

void printDist() {
  //  Serial.println(sizeof(marroniDist));
  for (int i = 0; i < sizeof(marroniDist) / sizeof(marroniDist[0]); i++) {
    if (marroniDist[i] > 0) {
      Serial.print("gr. ");
      Serial.print(i );
      Serial.print(" c: " );
      Serial.println(marroniDist[i]);
    }
  }
}
