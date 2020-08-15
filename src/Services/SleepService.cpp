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
	Serial.println(settings()->sleepTime);
	if(settings()->sleepTime > 0){
		setInactivityCallback(settings()->sleepTime*1000000, startLightSleep);
	}
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
	if(settings()->shutdownTime > 0){
		instance->setInactivityCallback(settings()->shutdownTime*1000000, shutdown);
	}
}
void SleepService::wakeLightSleep()
{
	Serial.println("wakeLightSleep");
	instance->display->getTft()->writecommand(17);

	delay(40); // give the display some time to wake up from sleep

	runningContext->start();
	// digitalWrite(BL_PIN, 0);
	
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