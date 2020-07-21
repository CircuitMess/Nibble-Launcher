#ifndef BYTEBOI_GAMESCROLLER_H
#define BYTEBOI_GAMESCROLLER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>
#include <Util/Vector.h>

class Sprite;
class Launcher;
class GameImage;

class GameScroller : public UpdateListener {
public:
	GameScroller(Sprite* canvas, Launcher* launcher);

	void draw();
	void update(uint micros) override;

	uint8_t prev();
	uint8_t next();

private:
	const uint8_t gutter = 20;
	const uint8_t width = 60;
	const uint8_t origin;
	const float speed = 150;

	Sprite* canvas;
	Launcher* launcher;
	Vector<GameImage*> games;

	uint selectedGame = 0;
	enum Direction { PREV, NEXT } direction;
	float delta = 0;
	bool queued = false;
	uint8_t multiplier = 1;

	void repos();

	void selectNext();
	void selectPrev();

	GameImage* getCGame();
	GameImage* getLGame();
	GameImage* getRGame();
	GameImage* getLLGame();
	GameImage* getRRGame();
};


#endif //BYTEBOI_GAMESCROLLER_H
