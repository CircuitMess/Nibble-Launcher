#ifndef NIBBLE_HWTEST_H
#define NIBBLE_HWTEST_H
#include <Display/Display.h>
#include <Util/Vector.h>

struct Test {
	bool (*test)();
	const char* name;
};

class HardwareTest
{
public:
	HardwareTest(Display& display);
	void start();

private:
	Sprite *canvas;
	Display *display;

	static HardwareTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);

	static bool buttonsTest();
	static bool voltageTest();

	void beeping();
};


#endif