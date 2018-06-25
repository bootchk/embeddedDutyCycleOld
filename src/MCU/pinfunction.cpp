#include "pinfunction.h"

/*
 * Choose mcu family and chip.
 * Must precede SPI.h
 * This configures pins for SPI interface to the default for the Launchpad i.e. the dev kit board.
 * I.E. configures a board.h file.
 *
 * A hack to get a clean compile in Eclipse IDE.
 * !!! Not necessarily correct, they just eliminate compiler errors.
 * Not needed if compiling in Energia IDE (where the Board Manager does magic.)
 * Proper paths also necessary.
 */
#define __MSP430__
#define __MSP430FR4133__ 1



// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h


// rename from hal namespace to alarmLib namespace
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN6

// TODO
#define RTCSelectPort 1
#define RTCSelectPin 2





void PinFunction::configureAlarmPinPullupLoToHiInterrupt() {
	GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
    GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
	GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
}

void PinFunction::clearAlarmInterruptOnPin() {
	GPIO_clearInterrupt(AlarmSignalPort, AlarmSignalPin);
}

bool PinFunction::isAlarmPinHigh() {
	return (GPIO_getInputPinValue(AlarmSignalPort, AlarmSignalPin) != 0);
}







void PinFunction::configureSelectSPIPin() {
	GPIO_setAsOutputPin(RTCSelectPort, RTCSelectPin);
}


void PinFunction::selectSPISlave(){
	GPIO_setOutputLowOnPin(RTCSelectPort, RTCSelectPin);
}


void PinFunction::deselectSPISlave(){
	GPIO_setOutputHighOnPin(RTCSelectPort, RTCSelectPin);
}

