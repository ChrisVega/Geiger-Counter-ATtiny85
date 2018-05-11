#include <font6x8.h>
#include <font8x16.h>
#include <ssd1306xled.h>
#include <ssd1306xled8x16.h>
#include "stalker.h"
/*
   Made using the RH electronics Geiger kit although any circuit should work
   Developed for ATtiny85 using SSD1306 OLED
*/
#define Conv_Fac 0.0057 // Conversion factor for CPM to uSV/h !!!For the SBM-20 Soviet G-M Tube!!!

const long Period = 10000;  // Time for which particles are counted in milliseconds, higher time is more accurate but taker longer
const float multiplier = 60000.0 / Period; // muultiplied by the count to esimate CPM
long count = 0;
long CPM = 0;
long lastmeasurement_time = 0;
float Dose = 0.0;
String CPMdis, DoseDis;

void setup() {
  sei();
  _delay_ms(40);
  SSD1306.ssd1306_init();
  SSD1306.ssd1306_fillscreen(0x00);
  SSD1306.ssd1306_draw_bmp(0, 0, 128, 8, stalker);
  GIMSK  |= (1 << PCIE); // enable PCINT interrupt
  PCMSK  |= (1 << PCINT4); // PB4

}

ISR(PCINT0_vect)
{
  /*
     When a radioactive particle passes though the G-M tube there will be a pulse from the Geiger counter's output pin
     RH electronic's circuit sends HIGH LOW HIGH but this will work with LOW HIGH LOW
     The pulse is counted and multiplied by the multiplier to estimate the count per minute
  */
  byte pinState;
  pinState = (PINB >> PINB4) & 1; // read PINB register to check state of pins
  if (pinState > 0) { // will return 1 if high, only acts on rising edge
    count++;
  }
}

void loop() {
  if (millis() - lastmeasurement_time > Period) {
    CPM = multiplier * count; // Estimation of CPM
    Dose = CPM * Conv_Fac; // Doesage is esimated by multiplying the CPM by your tube's conversion factor
    CPMdis = String(CPM);
    CPMdis = String("CPM: " + CPMdis);
    DoseDis = String(Dose, 4);
    DoseDis = String("Dose: " + DoseDis + "uSv/h");
    lastmeasurement_time = millis();
    SSD1306.ssd1306_fillscreen(0x00);
    SSD1306.ssd1306_setpos(0, 1);
    SSD1306.ssd1306_string_font6x8(CPMdis.c_str());
    SSD1306.ssd1306_setpos(0,3);
    SSD1306.ssd1306_string_font6x8(DoseDis.c_str());
    count = 0;
  }
}
