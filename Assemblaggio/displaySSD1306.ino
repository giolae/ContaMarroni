
void displaySetLayout1() {
#ifdef _Adafruit_SSD1306_H_
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel bilancia
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.setCursor(0, 0);
  display.print(F("azione   :"));
  display.setCursor(0, 10);
  display.print(F("marroni  :"));
  display.setCursor(0, 20);
  display.print(F("peso     :"));
  display.setCursor(0, 30);
  display.print(F("pezzatura:"));

  display.display();
#endif
}


void displayUpdateLayout1() {
#ifdef _Adafruit_SSD1306_H_

  displaySetLayout1();

  display.setTextColor(WHITE,BLACK);
  
  display.setCursor(60, 0);
  display.print(currentAction);
  display.setCursor(60, 10);
  display.print(marroniCounter);
  display.setCursor(60, 20);
  display.print(marroniGrammi);
  display.setCursor(60, 30);
  display.print(marroniPezzatura);

  display.display();

  Serial.print(F("marroniCounter: ") );
  Serial.println( marroniCounter);

  Serial.print(F("marroniCorrente: ") );
  Serial.println( marroniCorrente);

  Serial.print(F("marroniGrammi: ") );
  Serial.println(marroniGrammi);

  Serial.print(F("marroniPezzatura: ") );
  Serial.println(marroniPezzatura );

#endif

}
