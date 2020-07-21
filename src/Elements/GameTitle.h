#ifndef BYTEBOI_GAMETITLE_H
#define BYTEBOI_GAMETITLE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>

class Sprite;

class GameTitle : public UpdateListener {
public:
	GameTitle(Sprite* canvas);

	void change(const char* newText);

	void update(uint micros) override;
	void draw();

private:
	const uint8_t width = 80;
	const uint8_t height = 20;
	const int8_t x;
	const int8_t y;
	const float speed = 130;
	const uint8_t overHide = 10;

	Sprite* canvas;
	const char* text = nullptr;

	float currentY;
	enum State { DOWN, UP } state;
	const char* changeTo = nullptr;

};


#endif //BYTEBOI_GAMETITLE_H
