// example code for Profiling Arduino Code
// http://www.dudley.nu/arduino_profiling
// William F. Dudley Jr.
// 2014 January 5

#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

// this can be used to turn profiling on and off
#define PROFILING 1
// this needs to be true in at least ONE .c, .cpp, or .ino file in your sketch
#define PROFILING_MAIN 1
// override the number of bins
#define MAXPROF 8
#include "profiling.h"

// stuff used for time keeping in our ISR
volatile unsigned int int_counter;
volatile unsigned char seconds, minutes;
unsigned int tcnt2; // used to store timer value

volatile long long_waste;

// because this has "PF(2)", when the processor is executing here, it
// will cause the value in bin "2" to increase.
// "__attribute__ ((noinline))" needed to prevent inlining of these
// trivial functions.  The inlining by the compiler broke the profiling.
void __attribute__ ((noinline)) big_cpu_fn_1 (void) {
long lo;
    PF(2);
    for(lo = 1L ; lo < 8600L ; lo++) {
	long_waste += lo;
    }
    // DVn("1 lw ", long_waste);
}

// because this has "PF(3)", when the processor is executing here, it
// will cause the value in bin "3" to increase.
// "__attribute__ ((noinline))" needed to prevent inlining of these
// trivial functions.  The inlining by the compiler broke the profiling.
void __attribute__ ((noinline)) big_cpu_fn_2 (void) {
long lo;
    PF(3);
    for(lo = 1L ; lo < 29900L ; lo++) {
	long_waste -= lo;
    }
    // DVn("2 lw ", long_waste);
}

void setup (void) {
#if PROFILING
    PF(0);
    prof_has_dumped = 0;
    clear_profiling_data();
#endif
    Serial.begin(9600);
    Serial.println("setup()");

    int_counter = 0;
    seconds = 0;
    minutes = 0;

    Serial.println("setupTimer()");
    setupTimer();
    pinMode(1, OUTPUT);
}

// the stuff inside "#if PROFILING" is necessary for the profiler; that's
// the results are dumped to the debug window, once a minute.
void loop (void) {
unsigned char op;

    PF(1);
    big_cpu_fn_1();	// goes in bin 2
    big_cpu_fn_2();	// goes in bin 3
    mydelay(20);	// goes in bin 7
    PF(1);	// rest of this should go in profiling bin 1
    op ^= 1;
    digitalWrite(1, op & 1);	// toggle a pin so we can see loop rate
    if(int_counter < 110) { DVn("sec ", seconds); }
#if PROFILING
    if(seconds % 60 == 3 && !prof_has_dumped) {
	dump_profiling_data();	// also clears profiling data
    }
    if(seconds % 60 == 4 && prof_has_dumped) {
	prof_has_dumped = 0;
    }
#endif
}
