#ifndef BYTEBOI_LAUNCHER_H
#define BYTEBOI_LAUNCHER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>
#include <Support/Context.h>

class Logo;
class GameTitle;
class GameScroller;
extern Context* runningContext;

class Launcher : public UpdateListener, public Context {
public:
	Launcher(Display* display);

	void update(uint micros) override;
	void start() override;
	void stop() override;
	void draw() override;

private:
	Display* display;
	Sprite* canvas;

	Logo* logo;
	GameTitle* title;
	GameScroller* scroller;

	uint8_t selectedGame = 0;

	void prev();
	void next();
};

#endif //BYTEBOI_LAUNCHER_H
