#include "GameTitle.h"

#include <Display/Sprite.h>
#include <Loop/LoopManager.h>

GameTitle::GameTitle(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2), y(canvas->height() - height - 4), currentY(y){
	currentY = canvas->height() + overHide;
}

void GameTitle::change(const char* newText){
	bool wasChanging = changeTo != nullptr;

	state = DOWN;
	changeTo = newText;

	if(wasChanging) return;
	LoopManager::addListener(this);
}

void GameTitle::loop(uint micros){
	currentY += speed * (micros / 1000000.0f) * (state == DOWN ? 1.0f : -1.0f);

	if(state == DOWN && currentY >= canvas->height() + overHide){
		currentY = canvas->height() + overHide; // correction for first invoke, when micros is exceptionally yuge

		text = changeTo;
		state = UP;
	}else if(state == UP && currentY <= y){
		LoopManager::removeListener(this);
		changeTo = nullptr;
		currentY = y;
	}
}

void GameTitle::draw(){
	if(text == nullptr) return;
	if(currentY > canvas->height()) return;

	canvas->setTextSize(2);
	canvas->setTextFont(1);
	canvas->setTextColor(TFT_WHITE);
	canvas->setCursor(0, currentY);
	canvas->printCenter(text);
}
