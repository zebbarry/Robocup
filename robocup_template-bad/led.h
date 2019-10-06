//************************************
//         led.h     
//************************************

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

void led_init(void);

void led_on(int led_pin);

void led_off(int led_pin);

bool led_toggle(int led_pin);

void led_set(int red_pin, int green_pin, int blue_pin, bool red, bool green, bool blue);

#endif /* LED_H_ */
