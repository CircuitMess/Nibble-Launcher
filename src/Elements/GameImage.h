#ifndef BYTEBOI_GAMEIMAGE_H
#define BYTEBOI_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>

class Sprite;

class GameImage {
public:
	GameImage(Sprite* canvas, const uint16_t* icon);

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void draw() const;

private:
	int16_t x;
	int16_t y;
	const uint16_t* icon;

	Sprite* canvas;
};


#endif //BYTEBOI_GAMEIMAGE_H
