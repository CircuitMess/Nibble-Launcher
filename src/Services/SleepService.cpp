#include "SleepService.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "BatteryService.h"

SleepService* SleepService::instance = nullptr;

SleepService::SleepService(Display& display) : display(&display)
{
	instance = this;
}
void SleepService::start()
{
	if(settings()->sleepTime > 0){
		setInactivityCallback(settings()->sleepTime*1000000, startLightSleep);
	}else{
		setInactivityCallback(0, nullptr);
	}

	Input::getInstance()->setAnyKeyCallback([](){
		instance->inactivityCheck = 0;
	});
}
void SleepService::startLightSleep()
{
	if(settings()->sleepTime == 0)
	{
		instance->setInactivityCallback(0, nullptr);
		return;
	}

	for(uint8_t i = 0; i < instance->onSleepCallbacks.size(); i++)
	{
		instance->onSleepCallbacks[i]();
	}

	runningContext->stop();
	LoopManager::removeListener(instance);
	LoopManager::loop();
	LoopManager::addListener(instance);

	instance->display->getBaseSprite()->clear(TFT_BLACK);
	instance->display->commit();
	Nibble.setBacklight(false);

	Input::getInstance()->setAnyKeyCallback(wakeLightSleep, 1);
	instance->sleepStatus = 1;
	if(settings()->shutdownTime > 0){
		instance->setInactivityCallback(settings()->shutdownTime*1000000, shutdown);
	}
	else
	{
		instance->setInactivityCallback(0, nullptr);
	}
}
void SleepService::wakeLightSleep()
{
	instance->sleepStatus = 0;
	Nibble.setBacklight(true);

	delay(100); // give the display some time to wake up from sleep

	runningContext->draw();
	if(BatteryService::getInstance()->modalShown()){
		BatteryService::getInstance()->draw();
		instance->display->commit();
	}else{
		instance->display->commit();
		runningContext->start();
	}

	instance->setInactivityCallback(settings()->sleepTime*1000000, startLightSleep);
	Input::getInstance()->setAnyKeyCallback([](){
		instance->inactivityCheck = 0;
	});

	for(uint8_t i = 0; i < instance->onWakeupCallbacks.size(); i++)
	{
		instance->onWakeupCallbacks[i]();
	}
}
void SleepService::shutdown()
{

	Nibble.shutdown();
}

void SleepService::loop(uint _time)
{
	if(sleepStatus)
	{
		delay(100);
	}
	if(inactivityCheck)
	{
		inactivityTime+=_time;
		if(inactivityTime >= inactivityCallbackTime && inactivityCallback != nullptr)
		{
			inactivityCallback();
		}
	}else{
		inactivityTime = 0;
	}
	inactivityCheck = 1;
}

void SleepService::setInactivityCallback(uint _time, void(*callback)())
{
	inactivityCallback = callback;
	inactivityCallbackTime = _time;
	inactivityTime = 0;
}
SleepService* SleepService::getInstance()
{
	return instance;
}
void SleepService::addOnSleepCallback(void(*callback)())
{
	onSleepCallbacks.push_back(callback);
}
void SleepService::addOnWakeupCallback(void(*callback)())
{
	onWakeupCallbacks.push_back(callback);
}

bool SleepService::isSleeping(){
	return sleepStatus;
}
