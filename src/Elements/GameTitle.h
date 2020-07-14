#ifndef BYTEBOI_GAMETITLE_H
#define BYTEBOI_GAMETITLE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>

class GameTitle : public UpdateListener {
public:
	GameTitle(Sprite* canvas);

	void change();

	void update(uint micros) override;
	void draw();

private:
	const uint8_t width = 80;
	const uint8_t height = 20;
	const int8_t x;
	const int8_t y;
	const float speed = 100;
	const uint8_t overHide = 10;

	Sprite* canvas;

	float currentY;
	enum State { DOWN, UP } state;
	bool changeTo = false; // replace with bitmap ptr

};


#endif //BYTEBOI_GAMETITLE_H
