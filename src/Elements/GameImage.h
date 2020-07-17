#ifndef BYTEBOI_GAMEIMAGE_H
#define BYTEBOI_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>

class Sprite;

class GameImage {
public:
	GameImage(Sprite* canvas);

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void setColor(Color color);

	void draw() const;

private:
	int16_t x;
	int16_t y;
	Color color = TFT_GREEN;

	Sprite* canvas;
};


#endif //BYTEBOI_GAMEIMAGE_H
