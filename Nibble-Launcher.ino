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
#include "src/HardwareTest.h"
#include "src/SerialID.h"

#include <ESP8266WiFi.h>

Launcher* launcher;
BatteryService* batteryService;
SleepService* sleepService;

Display* display;

void setup(){
	Serial.begin(115200);
	Serial.println();
	Nibble.begin();
	display = Nibble.getDisplay();

	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();

#ifdef DEBUG_FLAG
	uint8_t portRead = 0;

	if(Nibble.getExpander()){
		for(uint8_t i = 0; i < 7; i++){
			Nibble.getExpander()->pinMode(i, INPUT_PULLUP);
		}

		portRead = Nibble.getExpander()->portRead();
	}else{
		Nibble.getInput()->loop(0);

		for(int i = 0; i < 7; i++){
			portRead |= (~Nibble.getInput()->getButtonState(i) & 1) << i;
		}
	}

	portRead = portRead & 0b01111111;
	if(!portRead && !settings()->calibrated)
	{
		LoopManager::addListener(new SerialID);
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

	Nibble.setBacklight(true);
}

void loop(){
	LoopManager::loop();
}