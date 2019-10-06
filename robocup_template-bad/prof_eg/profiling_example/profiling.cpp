// example code for Profiling Arduino Code
// http://www.dudley.nu/arduino_profiling
// William F. Dudley Jr.
// 2014 January 5

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>
// this can be used to turn profiling on and off
#define PROFILING 1
// override the number of bins
#define MAXPROF 8
#include "profiling.h"

// more handy macros but unused in this example
#define InterruptOff  do{TIMSK2 &= ~(1<<TOIE2)}while(0)
#define InterruptOn  do{TIMSK2 |= (1<<TOIE2)}while(0)

// stuff used for time keeping in our ISR
extern volatile unsigned int int_counter;
extern volatile unsigned char seconds, minutes;
extern unsigned int tcnt2; // used to store timer value

// Arduino runs at 16 Mhz, so we have 1000 overflows per second...
// this ISR will get hit once a millisecond
ISR(TIMER2_OVF_vect) {

    int_counter++;
    if (int_counter == 1000) {
	seconds++;
	int_counter = 0;
	if(seconds == 60) {
	    seconds = 0;
	    minutes++;
	}
    }
#if PROFILING
    prof_array[prof_line]++;
#endif
    TCNT2 = tcnt2;  // reset the timer for next time
}


// Timer setup code borrowed from Sebastian Wallin
// http://popdevelop.com/2010/04/mastering-timer-interrupts-on-the-arduino/
// further borrowed from: http://www.desert-home.com/p/super-thermostat.html
void setupTimer (void) {
  //Timer2 Settings:  Timer Prescaler /1024
  // First disable the timer overflow interrupt while we're configuring
  TIMSK2 &= ~(1<<TOIE2);
  // Configure timer2 in normal mode (pure counting, no PWM etc.)
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
  // Select clock source: internal I/O clock
  ASSR &= ~(1<<AS2);
  // Disable Compare Match A interrupt enable (only want overflow)
  TIMSK2 &= ~(1<<OCIE2A);

  // Now configure the prescaler to CPU clock divided by 128
  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
  TCCR2B &= ~(1<<CS21);             // Clear bit

  /* We need to calculate a proper value to load the timer counter.
   * The following loads the value 131 into the Timer 2 counter register
   * The math behind this is:
   * (CPU frequency) / (prescaler value) = 125000 Hz = 8us.
   * (desired period) / 8us = 125.
   * MAX(uint8) - 125 = 131;
   */
  /* Save value globally for later reload in ISR */
  tcnt2 = 131;

  /* Finally load end enable the timer */
  TCNT2 = tcnt2;
  TIMSK2 |= (1<<TOIE2);
  sei();
}
