#include "SleepService.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Nibble.h>
#include "../SettingsMenu/SettingsStruct.hpp"
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

	// runningContext->stop();
	// instance->display->getTft()->writecommand(16);
	I2cExpander::getInstance()->pinWrite(BL_PIN, 0);

	Input::getInstance()->setAnyKeyCallback(wakeLightSleep, 1);

	if(settings()->shutdownTime > 0){
		instance->setInactivityCallback(settings()->shutdownTime*1000000, shutdown);
	}
	else
	{
		instance->setInactivityCallback(0, nullptr);
	}
	instance->sleepStatus = 1;
}
void SleepService::wakeLightSleep()
{
	instance->sleepStatus = 0;
	// instance->display->getTft()->writecommand(17);

	// delay(100); // give the display some time to wake up from sleep

	// runningContext->start();
	// runningContext->draw();
	// instance->display->commit();

	I2cExpander::getInstance()->pinWrite(BL_PIN, 1);

	instance->setInactivityCallback(settings()->sleepTime*1000000, startLightSleep);
	Input::getInstance()->setAnyKeyCallback([](){
		instance->inactivityCheck = false;
	});

	for(uint8_t i = 0; i < instance->onWakeupCallbacks.size(); i++)
	{
		instance->onWakeupCallbacks[i]();
	}
}
void SleepService::shutdown()
{

	instance->display->getTft()->writecommand(16);
	I2cExpander::getInstance()->pinWrite(BL_PIN, 0);
	I2cExpander::getInstance()->portConfig(0xFFFF);
	ESP.deepSleep(0);
}

void SleepService::update(uint _time)
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