#include <Util/Settings.h>

#ifndef NIBBLE_SETTINGSSTRUCT_H
#define NIBBLE_SETTINGSSTRUCT_H

struct SettingsStruct {
	int sleepTime;
	int shutdownTime;
	bool audio;
};

SettingsStruct* settings();

#endif