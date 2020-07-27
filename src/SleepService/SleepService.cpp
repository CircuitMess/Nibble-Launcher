#include "SleepService.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include "../Nibble.hpp"

SleepService* SleepService::instance = nullptr;

SleepService::SleepService(Display& display) : display(&display)
{
	instance = this;
}
void SleepService::start()
{
	Input::getInstance()->setInactivityCallback(lightSleepTimeout, startLightSleep);
}
void SleepService::startLightSleep()
{
	runningContext->stop();
	// digitalWrite(BL_PIN, 0);
	instance->display->getTft()->writecommand(16);
	I2cExpander::getInstance()->pinWrite(BL_PIN, 0);
	Input::getInstance()->setActivityCallback(wakeLightSleep);
	Input::getInstance()->setInactivityCallback(instance->shutdownTimeout, shutdown);
}
void SleepService::wakeLightSleep()
{
	runningContext->start();
	// digitalWrite(BL_PIN, 0);
	instance->display->getTft()->writecommand(17);
	
	runningContext->draw();
	instance->display->commit();
	I2cExpander::getInstance()->pinWrite(BL_PIN, 1);
	Input::getInstance()->setActivityCallback(nullptr);
	Input::getInstance()->setInactivityCallback(instance->lightSleepTimeout, startLightSleep);
}
void SleepService::shutdown()
{
	instance->display->getTft()->writecommand(16);
	I2cExpander::getInstance()->pinWrite(BL_PIN, 0);
	I2cExpander::getInstance()->portConfig(0xFFFF);
	ESP.deepSleep(0);
}
