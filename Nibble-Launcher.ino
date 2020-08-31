#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Audio/Piezo.h>

#include "src/Launcher.h"
#include "src/Services/BatteryService.h"
#include "src/Services/SleepService.h"
#include "src/SettingsMenu/SettingsStruct.hpp"
#include "src/HardwareTest.h"
#include "src/SerialID.h"

#include <ESP8266WiFi.h>

Launcher* launcher;
BatteryService* batteryService;
SleepService* sleepService;

Display* display;

#define DEBUG_FLAG

void setup(){
	Serial.begin(115200);
	Serial.println();
	Nibble.begin();
	display = Nibble.getDisplay();

	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();

	Piezo.begin(BUZZ_PIN);

	if(!Settings::init(new SettingsStruct, sizeof(SettingsStruct))){
		settings()->shutdownTime = 300; //5 minutes
		settings()->sleepTime = 30; //30 seconds
		settings()->audio = true; //audio on
		settings()->calibrated = false;
		Settings::store();
	}

#ifdef DEBUG_FLAG
	LoopManager::addListener(new SerialID);

	for(uint8_t i = 0; i < 7; i++)
	{
		Nibble.getExpander()->pinMode(i, INPUT_PULLUP);
	}
	uint8_t portRead = Nibble.getExpander()->portRead() & 0b01111111;

	if(!portRead && !settings()->calibrated)
	{
		HardwareTest test(*Nibble.getDisplay());
		test.start();
	}

#endif

	Piezo.setMute(!settings()->audio);

	batteryService = new BatteryService(*display);
	sleepService = new SleepService(*display);

	LoopManager::addListener(batteryService);
	LoopManager::addListener(Input::getInstance());

	launcher = new Launcher(display, batteryService);
	runningContext = launcher;

	launcher->unpack();
	launcher->start();
	sleepService->start();
	LoopManager::addListener(sleepService);
}

void loop(){
	LoopManager::loop();
}