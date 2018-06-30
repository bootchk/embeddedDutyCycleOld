
#include "led.h"


// For compiling in some IDE's where these are not defined earlier
#ifndef __MSP430__
#define __MSP430__
#define __MSP430FR4133__
// #define MSPEXP430FR2433LP
#endif


// DriverLib
#include <gpio.h>



// P1.0
#define APP_LED_PORT     GPIO_PORT_P1
#define APP_LED_PIN      GPIO_PIN0


void LED::configure() {
	GPIO_setAsOutputPin(APP_LED_PORT,    APP_LED_PIN);
}


/*
 * mcu sources current to LED: high is on
 */
void LED::turnOff(){
	GPIO_setOutputLowOnPin(APP_LED_PORT,    APP_LED_PIN);
}

void LED::turnOn(){
	GPIO_setOutputHighOnPin(APP_LED_PORT,    APP_LED_PIN);
}
