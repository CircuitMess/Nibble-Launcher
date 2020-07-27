#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Update/UpdateManager.h>
#include <Support/Context.h>

#include "src/Nibble.hpp"
#include "src/Launcher.h"
#include "src/BatteryService/BatteryService.h"

Display display(128, 128, -1, 0);
I2cExpander i2c;
InputI2C buttons(&i2c);

Launcher* launcher;
Context* runningContext = nullptr;
BatteryService* batteryService;
void setup(){
	Serial.begin(115200);
	i2c.begin(0x74, 4, 5);
	i2c.pinMode(8, OUTPUT);
	i2c.pinWrite(8, 1);
	
	display.begin();

	batteryService = new BatteryService(display);
	UpdateManager::addListener(batteryService);
	UpdateManager::addListener(&buttons);
	launcher = new Launcher(&display, batteryService);
	launcher->unpack();
	launcher->start();
}

void loop(){
	UpdateManager::update();
}