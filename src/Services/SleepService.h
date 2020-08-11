#ifndef SLEEPSERVICE_NIBBLE_H
#define SLEEPSERVICE_NIBBLE_H

#include <Update/UpdateListener.h>
#include <Support/Context.h>
#include <Display/Display.h>
#include <UI/Image.h>

extern Context* runningContext;
class SleepService : public UpdateListener
{
public:
	SleepService(Display& display);
	void start();
	void update(uint _time);
private:
	static void startLightSleep();
	static void wakeLightSleep();
	static void shutdown();
	void setInactivityCallback(uint _time, void(*callback)());

	Display* display;

	static SleepService* instance;

	void(*inactivityCallback)(void) = nullptr; 
	uint inactivityTime = 0;
	bool inactivityCheck = 1;
	uint inactivityCallbackTime = 0;
};

#endif