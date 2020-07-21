#ifndef BYTEBOI_LAUNCHER_H
#define BYTEBOI_LAUNCHER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>

class Logo;
class GameTitle;
class GameScroller;

class Launcher : public UpdateListener {
public:
	Launcher(Display* display);

	void update(uint micros) override;

private:
	Display* display;
	Sprite* canvas;

	Logo* logo;
	GameTitle* title;
	GameScroller* scroller;

	uint8_t selectedGame = 0;

	void prev();
	void next();

	void draw();

};


#endif //BYTEBOI_LAUNCHER_H
