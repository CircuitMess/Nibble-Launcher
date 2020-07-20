#include "Launcher.h"
#include "ByteBoi.hpp"
#include <Input/Input.h>

#include "GameScroller.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"

Launcher* instance = nullptr;

Launcher::Launcher(Display* display) : display(display), canvas(display->getBaseSprite()),
		scroller(new GameScroller(canvas, this)), logo(new Logo(canvas)), title(new GameTitle(canvas)){

	instance = this;
	canvas->setChroma(TFT_TRANSPARENT);

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance->switching == RIGHT){
			instance->queue = RIGHT;
			return;
		}

		instance->queue = NONE;
		instance->next();
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance->switching == LEFT){
			instance->queue = LEFT;
			return;
		}

		instance->queue = NONE;
		instance->prev();
	});
}

void Launcher::prev(){
	switching = LEFT;
	instance->title->change();
	instance->scroller->prev();
}

void Launcher::next(){
	switching = RIGHT;
	instance->title->change();
	instance->scroller->next();
}

void Launcher::switched(){
	if(queue == NONE){
		switching = NONE;
	}else{
		if(queue == RIGHT){
			next();
		}else{
			prev();
		}

		queue = NONE;
	}
}

void Launcher::update(uint micros){
	draw();
	display->commit();
}

void Launcher::draw(){
	canvas->clear(TFT_BLACK);
	logo->draw();
	title->draw();
	scroller->draw();
}
