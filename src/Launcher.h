#ifndef BYTEBOI_LAUNCHER_H
#define BYTEBOI_LAUNCHER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>

class Launcher : public UpdateListener {
public:
	Launcher(Display* display);

	void update(uint micros) override;

private:
	Display* display;
	Sprite* canvas;

	void draw();

};


#endif //BYTEBOI_LAUNCHER_H
