//************************************
//         led.h     
//************************************

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

#define RED   42
#define GREEN 44
#define BLUE  43

void led_init(void);

void led_on(int led_pin);

void led_off(int led_pin);

bool led_toggle(int led_pin);

void led_write(int red_pin, int green_pin, int blue_pin);

#endif /* LED_H_ */
