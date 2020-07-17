#include "GameImage.h"

GameImage::GameImage(Sprite* canvas) : canvas(canvas){

}

void GameImage::draw() const {
	canvas->fillRect(x, y, 60, 60, color);
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

void GameImage::setColor(Color color){
	GameImage::color = color;
}
