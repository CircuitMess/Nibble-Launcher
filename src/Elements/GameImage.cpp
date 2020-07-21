#include "GameImage.h"

GameImage::GameImage(Sprite* canvas, const uint16_t* icon) : canvas(canvas), icon(icon){

}

void GameImage::draw() const {
	canvas->drawIcon(icon, x, y, 60, 60, 1, TFT_TRANSPARENT);
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
