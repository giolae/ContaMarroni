void doTare() {
  Serial.println(F("doTare START"));

#if defined(HX711_h)
  // set the OFFSET value for tare weight; times = how many times to read the tare value
  //void tare(byte times = 10);
  bilancia.tare(15);
#endif

  Serial.println(F("Tare complete"));
  Serial.println(F("doTare END"));
  currentAction = NULL;
}
