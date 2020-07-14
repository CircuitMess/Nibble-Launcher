#include "Launcher.h"
#include "ByteBoi.hpp"
#include <Input/Input.h>

#include "Elements/Logo.h"
#include "Elements/GameTitle.h"

Launcher* instance = nullptr;

Launcher::Launcher(Display* display) : display(display), canvas(display->getBaseSprite()),
		logo(new Logo(canvas)), title(new GameTitle(canvas)){

	instance = this;

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		Serial.println("Switching game");
		instance->title->change();
	});
}

void Launcher::update(uint micros){
	draw();
	display->commit();
}

void Launcher::draw(){
	canvas->clear(TFT_BLACK);
	logo->draw();
	title->draw();
}
