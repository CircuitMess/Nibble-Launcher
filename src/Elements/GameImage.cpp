#include "GameImage.h"
#include <Display/Sprite.h>
#include <Display/Color.h>

GameImage::GameImage(Sprite* canvas, const uint16_t* icon) : canvas(canvas), icon(icon){

}

void GameImage::draw() const {
	canvas->drawIcon(icon, x, y, 30, 30, 2, TFT_BLACK);
}

int16_t GameImage::getX() const{
	return x;
}

void GameImage::setX(int16_t x){
	GameImage::x = x;
}

int16_t GameImage::getY() const{
	return y;
}

void GameImage::setY(int16_t y){
	GameImage::y = y;
}
