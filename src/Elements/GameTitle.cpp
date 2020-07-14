#include "GameTitle.h"

#include <Display/Sprite.h>
#include <Update/UpdateManager.h>

GameTitle::GameTitle(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2), y(canvas->height() - height - 5), currentY(y){

}

void GameTitle::change(){
	state = DOWN;

	if(changeTo){
		return;
	}

	changeTo = true;
	UpdateManager::addListener(this);
}

void GameTitle::update(uint micros){
	currentY += speed * (micros / 1000000.0f) * (state == DOWN ? 1.0f : -1.0f);

	if(state == DOWN && currentY > canvas->height() + overHide){
		// switch sprite
		state = UP;
	}else if(state == UP && currentY <= y){
		UpdateManager::removeListener(this);
		changeTo = false;
		currentY = y;
	}
}

void GameTitle::draw(){
	canvas->fillRect(x, currentY, width, height, TFT_GREEN);
}
