#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Update/UpdateManager.h>

#include "src/ByteBoi.hpp"
#include "src/Launcher.h"

Display display(128, 128, BL_PIN, 0);
I2cExpander i2c;
InputI2C buttons(&i2c);

Launcher* launcher;

void setup(){
	Serial.begin(115200);
	i2c.begin(0x74, 4, 5);
	display.begin();

	UpdateManager::addListener(&buttons);

	launcher = new Launcher(&display);
	UpdateManager::addListener(launcher);
}

void loop(){
	UpdateManager::update();
}