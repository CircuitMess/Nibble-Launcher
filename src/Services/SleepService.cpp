#include "SleepService.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include "../Nibble.hpp"
#include "../SettingsMenu/SettingsStruct.hpp"
SleepService* SleepService::instance = nullptr;

SleepService::SleepService(Display& display) : display(&display)
{
	instance = this;
}
void SleepService::start()
{
	Serial.println(settings()->sleepTime);
	setInactivityCallback(settings()->sleepTime*1000000, startLightSleep);
	Input::getInstance()->setAnyKeyCallback([](){
		instance->inactivityCheck = 0;
	});
}
void SleepService::startLightSleep()
{
	runningContext->stop();
	// digitalWrite(BL_PIN, 0);
	instance->display->getTft()->writecommand(16);
	I2cExpander::getInstance()->pinWrite(BL_PIN, 0);
	Input::getInstance()->setAnyKeyCallback(wakeLightSleep, 1);
	instance->setInactivityCallback(settings()->shutdownTime*1000000, shutdown);
}
void SleepService::wakeLightSleep()
{
	Serial.println("wakeLightSleep");
	runningContext->start();
	// digitalWrite(BL_PIN, 0);
	instance->display->getTft()->writecommand(17);
	
	runningContext->draw();
	instance->display->commit();
	I2cExpander::getInstance()->pinWrite(BL_PIN, 1);
	instance->setInactivityCallback(settings()->sleepTime*1000000, startLightSleep);
	Input::getInstance()->setAnyKeyCallback([](){
		instance->inactivityCheck = 0;
	});
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