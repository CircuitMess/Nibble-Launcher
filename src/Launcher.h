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

	void switched();

private:
	Display* display;
	Sprite* canvas;

	Logo* logo;
	GameTitle* title;
	GameScroller* scroller;

	bool switching = false;
	enum SwitchQueue { NONE, RIGHT, LEFT } queue = NONE;

	void prev();
	void next();

	void draw();

};


#endif //BYTEBOI_LAUNCHER_H
