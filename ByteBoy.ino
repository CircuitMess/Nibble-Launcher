#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputGPIO.h>
#include <Update/UpdateManager.h>
#include "src/MainMenu.h"

Display display(128, 128, 18, 0);
InputGPIO buttons;
Context* mainMenu = nullptr;

void setup(){
	Serial.begin(115200);
	display.begin();

	UpdateManager::addListener(&buttons);

	mainMenu = new MainMenu(display);
	mainMenu->unpack();
	mainMenu->start();
}

void loop(){
	UpdateManager::update();
	yield();
}
