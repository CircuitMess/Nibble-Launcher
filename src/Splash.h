#ifndef NIBBLE_SPLASH_H
#define NIBBLE_SPLASH_H

#include <Arduino.h>

class Sprite;
class Logo;
class GameTitle;
class GameScroller;

class Splash {
public:
	Splash(Sprite* canvas, Logo* logo, GameTitle* title, GameScroller* scroller);

	void update(uint micros);
	bool done();

private:
	const float hold = 0.5;
	const uint8_t logoStart = 50;
	const uint8_t logoEnd = 8;

	float f = 0;

	Sprite* canvas;

	Logo* logo;
	GameTitle* title;
	GameScroller* scroller;
};


#endif //NIBBLE_SPLASH_H
