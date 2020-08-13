#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Update/UpdateManager.h>
#include <Support/Context.h>
#include <Audio/Piezo.h>

#include "src/Nibble.hpp"
#include "src/Launcher.h"
#include "src/Services/BatteryService.h"
#include "src/Services/SleepService.h"
#include "src/SettingsMenu/SettingsStruct.hpp"

#include "src/HardwareTest.h"

Display display(128, 128, -1, 0);
I2cExpander i2c;
InputI2C buttons(&i2c);

Launcher* launcher;
Context* runningContext = nullptr;
BatteryService* batteryService;
SleepService* sleepService;

void setup(){
	display.begin();
	display.getBaseSprite()->clear(TFT_BLACK);
	display.commit();

	Serial.begin(115200);
	i2c.begin(0x74, 4, 5);
	i2c.pinMode(BL_PIN, OUTPUT);
	i2c.pinWrite(BL_PIN, 1);

	Piezo.begin(BUZZ_PIN);
	if(!Settings::init(new SettingsStruct, sizeof(SettingsStruct))){
		settings()->shutdownTime = 300; //5 minutes
		settings()->sleepTime = 30; //30 seconds
		settings()->audio = 1; //audio on
		settings()->calibrated = 0;
	}

#ifdef DEBUG_FLAG

	for(uint8_t i = 0; i < 7; i++)
	{
		i2c.pinMode(i, INPUT_PULLUP);
	}
	uint16_t portRead = i2c.portRead();
	// Serial.println();
	// Serial.println(portRead, BIN);
	// Serial.println(((portRead & 0b1111111) == 0) ? "all pressed" : "not all pressed");
	// Serial.printf("Calibrated: %d\n", settings()->calibrated);
	
	if(((portRead & 0b1111111) == 1) && !settings()->calibrated)
	{
		HardwareTest test = HardwareTest(display);
		test.start();
	}

#endif
	
	Piezo.setMute(!settings()->audio);

	batteryService = new BatteryService(display);
	sleepService = new SleepService(display);

	UpdateManager::addListener(batteryService);
	UpdateManager::addListener(&buttons);

	launcher = new Launcher(&display, batteryService);
	runningContext = launcher;

	launcher->unpack();
	launcher->start();
	sleepService->start();
	UpdateManager::addListener(sleepService);


}

void loop(){
	UpdateManager::update();
}