void doStart() {
  Serial.println(F("doStart START"));
  delay(500);
  marroniCounter = 0;
  marroniGrammi = 0;
  marroniPezzatura = 0;
  marroniDetected = false;
  marroniCorrente = 0;    // peso dell'ultimo marrone

  memset(marroniDist, 0, sizeof(marroniDist));

  displaySetLayout1() ;

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
      float _grammi = doReadWeight();

      int marroniCorrente = max(1, (int)_grammi - marroniGrammi);
      marroniCorrente = min(marroniCorrente, 39);
      marroniDist[marroniCorrente]++;

      marroniGrammi = _grammi;

      marroniPezzatura = marroniGrammi / marroniCounter;
      displayUpdateLayout1();
      //    drawChart();
    }
  } while (_input != 'e');
  Serial.println(F("doStart END"));
}
