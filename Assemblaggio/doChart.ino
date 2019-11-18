
void drawChart() {
#ifdef _Adafruit_SSD1306_H_
  display.clearDisplay();

  int _maxValue = 1;
  for (byte i = 1; i < sizeof(marroniDist); i++)
  {
    if (marroniDist[i] > _maxValue) {
      _maxValue = marroniDist[i] ;
    }
  }
  for (byte i = 1; i < sizeof(marroniDist); i++) {
    // The INVERSE color is used so rectangles alternate white/black
    int _v = (display.height() / 2) * marroniDist[i] / _maxValue;
    display.fillRect(i, display.height() , 3, display.height() - _v, SSD1306_INVERSE);
  }
  display.display();
#endif
}
