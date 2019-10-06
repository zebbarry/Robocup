// Arduino profiling code
// http://www.dudley.nu/arduino_profiling
// William F. Dudley Jr.
// 2014 January 5

#ifndef PROFILING_H

#if PROFILING

#ifndef MAXPROF
#define MAXPROF 30
#endif

// some handy macros for printing debugging values
#define DL(x) Serial.print(x)
#define DLn(x) Serial.println(x)
#define DV(m, v) do{Serial.print(m);Serial.print(v);Serial.print(" ");}while(0)
#define DVn(m, v) do{Serial.print(m);Serial.println(v);}while(0)

#define PF(n) do{prof_line=n;}while(0)
#define mydelay(v) do{PF(MAXPROF-1);delay(v);}while(0)

#if PROFILING_MAIN
    #define EXTERN
#else
    #define EXTERN extern
#endif

EXTERN volatile unsigned int prof_array[MAXPROF];
EXTERN volatile unsigned int prof_line;
EXTERN volatile unsigned char prof_has_dumped;

// Timer setup code borrowed from Sebastian Wallin
// http://popdevelop.com/2010/04/mastering-timer-interrupts-on-the-arduino/
// further borrowed from: http://www.desert-home.com/p/super-thermostat.html
void setupTimer (void);

#if PROFILING_MAIN

#define _DL(x) Serial.print(x)
#define _DLn(x) Serial.println(x)

void clear_profiling_data (void) {
unsigned char c;
    for(c = 0 ; c < MAXPROF ; c++) {
	prof_array[c] = 0;
    }
}

void dump_profiling_data(void) {
unsigned char c;
    prof_has_dumped = 1;
    PF(0);
    for(c = 0 ; c < MAXPROF ; c++) {
	_DL((int)c);
	_DL(" ");
	_DLn((unsigned int)prof_array[c]);
	prof_array[c] = 0;
    }
}
#endif

#else
#define PF(n)
#define mydelay(v) delay(v)
#endif

#define PROFILING_H 1
#endif
