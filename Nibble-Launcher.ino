#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Update/UpdateManager.h>
#include <Support/Context.h>
#include <Input/Input.h>
#include <Audio/Piezo.h>

#include "src/Launcher.h"
#include "src/Services/BatteryService.h"
#include "src/Services/SleepService.h"
#include "src/SettingsMenu/SettingsStruct.hpp"

#include <ESP8266WiFi.h>

Launcher* launcher;
Context* runningContext = nullptr;
BatteryService* batteryService;
SleepService* sleepService;

Display* display;

void setup(){
	Serial.begin(115200);
	Nibble.begin();
	display = Nibble.getDisplay();

	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();

	Piezo.begin(BUZZ_PIN);

	if(!Settings::init(new SettingsStruct, sizeof(SettingsStruct))){
		settings()->shutdownTime = 300; //5 minutes
		settings()->sleepTime = 30; //30 seconds
		settings()->audio = 1; //audio on
	}
	Piezo.setMute(!settings()->audio);

	batteryService = new BatteryService(*display);
	sleepService = new SleepService(*display);

	UpdateManager::addListener(batteryService);
	UpdateManager::addListener(Input::getInstance());

	launcher = new Launcher(display, batteryService);
	runningContext = launcher;

	launcher->unpack();
	launcher->start();
	sleepService->start();
	UpdateManager::addListener(sleepService);
}

void loop(){
	UpdateManager::update();
}