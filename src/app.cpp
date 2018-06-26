
#include "app.h"


void App::onPowerOnReset() {
	// TODO configure LED
	// initialize state
	// decide first alarm duration
}

void App::onWakeForAlarm() {
	// require app's GPIO still configured (LED)
	// TODO state transition
	// decide next alarm duration
}


unsigned int App::durationOfSleep() {
	// TODO get from state
	return 1;
}
