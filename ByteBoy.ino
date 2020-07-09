#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Update/UpdateManager.h>
#include <ArduinoJson.h>
#include <spiffs_api.h>
#include <gpio.h>
#include <avr/pgmspace.h>
#include "src/ByteBoy.hpp"
#include "src/MainMenu.h"

Display display(128, 128, BL_PIN, 0);
I2cExpander i2c;
InputI2C buttons(&i2c);
Context *mainMenu = nullptr;
void setup() {
	gpio_init();
	i2c.begin(0x74, 4, 5);
	Serial.begin(115200);
	display.begin();
	SPIFFS.begin();
	UpdateManager::addListener(&buttons);

	mainMenu = new MainMenu(display);
	mainMenu->unpack();
	mainMenu->start();
	mainMenu->stop();
	mainMenu->pack();
	mainMenu->unpack();
	mainMenu->start();
}
void loop() {
	UpdateManager::update();
	yield();
}