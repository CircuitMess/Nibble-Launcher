#ifndef GAME_INFO
#define GAME_INFO
#include <CircuitOS.h>
#include <Display/Display.h>
class Context;
struct GameInfo {
	const char* title;
	const char* description;
	const uint16_t* icon;
	Context* (*launch)(Display& display);
};
#endif
