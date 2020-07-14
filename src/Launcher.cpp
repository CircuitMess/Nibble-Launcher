#include "Launcher.h"
#include "ByteBoi.hpp"
#include <Input/Input.h>

Launcher* instance = nullptr;

Launcher::Launcher(Display* display) : display(display), canvas(display->getBaseSprite()){

	instance = this;

}

void Launcher::update(uint micros){
	draw();
	display->commit();
}

void Launcher::draw(){
	canvas->clear(TFT_BLACK);
}
