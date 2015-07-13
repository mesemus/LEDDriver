# LEDDriver

A small project for driving 10 different leds using ATMEGA328P (DIL) microprocessor dedicated as a PWM driver.
The driver communicates via serial interface, commands:

```
F<pin><val><pin><val>....\n
   - fade on selected pin from current value to the new one
S<pin><val><pin><val>....\n
   - set the value of the pins (without fading)
   
val is (pseudo)hexadecimal number:
  00 .. completely off
  01 - ff .. pwm on channel
  g0 .. completely on
```

This driver uses the SoftTablePWM library and is built in Eclipse, 
converting it to .ino should pose no problem.
