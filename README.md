# Geiger-Counter-ATtiny85
Geiger Counter using an Attiny85 to estimate radioactive dosage dosage.

Uses the SBM-20 Geiger–Müller tube.
You must use a 4.7kΩ pullup resistors on the Attiny85's SDA and SCL pins.

Made using the [RH-Electronics kit](http://www.rhelectronics.net/store/radiation-detector-geiger-counter-diy-kit-second-edition.html) but any circuit should work as long as there is a pulse when a particle is detected. 

## Circuit Diagram
The following is the circuit diagram needed for both screens.

![x](https://i.imgur.com/rjDOTEW.png)


## LCD Screen

LCD sreen that was used has an I2C backpack.

For a guide of how to get the LCD screen to work, and which library to use, follow this [users solution](https://arduino.stackexchange.com/questions/29546/attiny85-i2c-lcd). The LCD screen libaries can be very picky about the cores that are used. His solution should save you a few hours of hair pulling and fustration.

## OLED screen

OLED screen that was used was an sdd1306.

Library used was Defragster's [SSD1306OLED port from AVR TinuSaur to Arduino](https://github.com/Defragster/ssd1306xled).
