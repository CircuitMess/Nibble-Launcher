#ifndef SLEEPSERVICE_NIBBLE_H
#define SLEEPSERVICE_NIBBLE_H

#include <Update/UpdateListener.h>
#include <Support/Context.h>
#include <Display/Display.h>
#include <UI/Image.h>

extern Context* runningContext;
class SleepService
{
public:
	SleepService(Display& display);
	void start();
private:
	static void startLightSleep();
	static void wakeLightSleep();
	static void shutdown();

	Display* display;

	static SleepService* instance;
	const uint lightSleepTimeout = 1000000;
	const uint shutdownTimeout = 10000000;
};

#endif