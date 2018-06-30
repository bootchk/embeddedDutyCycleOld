
#include "app.h"

#include "LED/led.h"

#pragma PERSISTENT(ledState)
bool ledState = false;




void App::onPowerOnReset() {
	configureSleepingGPIO();

	// TODO later
	// TODO initialize state
	LED::turnOff();
	ledState = false;

	// decide first alarm duration
}

void App::onWakeForAlarm() {
	// require app's persistent GPIO still configured (LED)

	// app state transition
	// toggle LED every time we wake
	if (ledState) {
		LED::turnOff();
	}
	else {
		LED::turnOn();
	}
	ledState = ! ledState;


	// TODO decide next alarm duration
}


unsigned int App::durationOfSleep() {
	// TODO later, app get from state
	return 1;
}


void App::configureSleepingGPIO() {
	// App uses an LED during sleep
	LED::configure();
}
