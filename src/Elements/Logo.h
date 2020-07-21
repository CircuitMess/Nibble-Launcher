#ifndef BYTEBOI_LOGO_H
#define BYTEBOI_LOGO_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>

class Sprite;

class Logo {
public:
	Logo(Sprite* canvas);

	void draw();

private:
	const uint8_t width = 118;
	const uint8_t height = 20;
	const int8_t x;
	const int8_t y = 7;
	const float speed = 300;
	const float amplitude = 3;

	Sprite* canvas;
};


#endif //BYTEBOI_LOGO_H