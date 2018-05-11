#include <TinyWireM.h>
#include "LiquidCrystal_attiny.h"
/*
   Made using the RH electronics Geiger kit although any should work 
   Developed for ATtiny85 using LCD i2c
*/

#define Conv_Fac 0.0057 // Conversion factor for CPM to uSV/h !!!For the SBM-20 Soviet G-M Tube!!!

LiquidCrystal_I2C lcd(0x27, 16, 2);

const long Period = 10000;  // Time for which particles are counted in milliseconds, higher time is more accurate but taker longer
const float multiplier = 60000.0 / Period; // muultiplied by the count to esimate CPM
long count = 0;
long CPM = 0;
long lastmeasurement_time = 0;
float Dose = 0.0;

void setup() {
  sei();
  lcd.begin(16, 2);//initializes LCD
  TinyWireM.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Get out of here");//Start up message shown while first reading is being taken
  lcd.setCursor(0, 1);
  lcd.print("S.T.A.L.K.E.R.");
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
    lastmeasurement_time = millis();
    lcd.clear(); //Displays your dosage and CPM
    lcd.setCursor(0, 0);
    lcd.print("CPM=");
    lcd.setCursor(4, 0);
    lcd.print(CPM);
    lcd.setCursor(0, 1);
    lcd.print(Dose, 4);
    lcd.setCursor(6, 1);
    lcd.print(" uSv/h");
    count = 0;
  }
}
