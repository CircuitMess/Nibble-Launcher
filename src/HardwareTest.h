#ifndef NIBBLE_HWTEST_H
#define NIBBLE_HWTEST_H
#include <Display/Display.h>

class HardwareTest
{
public:
	HardwareTest(Display& display);
	void start();

private:
	Sprite *canvas;
	Display *display;

	double averageVoltage = 0.0;
	uint voltageSum = 0;
	const uint measurementsSize = 100;
	uint measurementCounter = 0;

	void voltageTest();
	void beeping();
};


#endif