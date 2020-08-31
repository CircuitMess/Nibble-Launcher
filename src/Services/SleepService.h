#ifndef SLEEPSERVICE_NIBBLE_H
#define SLEEPSERVICE_NIBBLE_H

#include <Loop/LoopListener.h>
#include <Support/Context.h>
#include <Display/Display.h>
#include <UI/Image.h>

extern Context* runningContext;
class SleepService : public LoopListener
{
public:
	SleepService(Display& display);
	void start();
	void loop(uint _time) override;
	static SleepService* getInstance();
	void addOnSleepCallback(void(*callback)());
	void addOnWakeupCallback(void(*callback)());
	bool isSleeping();

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
	bool sleepStatus = 0;

	Vector<void(*)()> onSleepCallbacks;
	Vector<void(*)()> onWakeupCallbacks;
};

#endif