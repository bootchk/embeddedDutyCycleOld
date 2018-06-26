
#include "app.h"


void App::onPowerOnReset() {
	// TODO later, app configure LED
	// initialize state
	// decide first alarm duration
}

void App::onWakeForAlarm() {
	// require app's GPIO still configured (LED)
	// TODO later, app state transition
	// decide next alarm duration
}


unsigned int App::durationOfSleep() {
	// TODO later, app get from state
	return 1;
}
