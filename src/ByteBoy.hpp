#ifndef BYTEBOY_HPP
#define BYTEBOY_HPP

#define BTN_UP 0
#define BTN_DOWN 1
#define BTN_LEFT 2
#define BTN_RIGHT 3
#define BTN_A 4
#define BTN_B 5
#define BTN_C 6
#define TT1 &TomThumb
#define BUZZ_PIN 12
#define BL_PIN 16
#define NO_KEY '\0'
#include <Arduino.h>
#include <Support/Context.h>
class Game : public Context
{
public:
	Game(Display &display,  char* title = nullptr, const unsigned short* gameIcon = nullptr, char* description = nullptr);
	static const unsigned short *getIcon();
	static const char* getDescription();
	static const char* getTitle();
private:
	const unsigned short *gameIcon;
	const char* description;
	const char* title;
	static Game* instance;
};

#endif