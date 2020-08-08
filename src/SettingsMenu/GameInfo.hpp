#ifndef GAME_INFO
#define GAME_INFO

class Context;
struct GameInfo {
	const char* title;
	const char* description;
	const uint16_t* icon;
	Context* (*launch)(Display& display);
};
#endif

#include <Arduino.h>
#include <Display/Display.h>
#include "../Bitmaps/settingsIcon.hpp"
#include "SettingsMenu.h"

const GameInfo SettingsInfo {
		"Settings",
		"Change your Nibble's settings.",
		settingsIcon,
		[](Display& display) -> Context* { return new SettingsMenu::SettingsMenu(display); }
};