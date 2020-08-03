#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Update/UpdateManager.h>
#include <Support/Context.h>

#include "src/Nibble.hpp"
#include "src/Launcher.h"
#include "src/Services/BatteryService.h"
#include "src/Services/SleepService.h"
Display display(128, 128, -1, 0);
I2cExpander i2c;
InputI2C buttons(&i2c);

Launcher* launcher;
Context* runningContext = nullptr;
BatteryService* batteryService;
SleepService* sleepService;
void setup(){
	Serial.begin(115200);
	i2c.begin(0x74, 4, 5);
	i2c.pinMode(BL_PIN, OUTPUT);
	i2c.pinWrite(BL_PIN, 1);
	
	display.begin();

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