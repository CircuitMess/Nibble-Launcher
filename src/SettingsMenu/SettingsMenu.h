#ifndef NIBBLE_SETTINGSMENU_H
#define NIBBLE_SETTINGSMENU_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/Input.h>
#include <Update/UpdateManager.h>
#include <Support/Context.h>
#include "../Nibble.hpp"
#include <UI/Image.h>
#include <Display/Sprite.h>
#include <UI/LinearLayout.h>
#include "Elements/SettingElement.h"
namespace SettingsMenu
{

class SettingsMenu: public Context, public UpdateListener
{
public:
	SettingsMenu(Display& Display);

	void draw() override;
	void start() override;
	void update(uint _time) override;
	void stop() override;
	void pack() override;
	
private:
	static SettingsMenu* instance;
	Display* display;
	Sprite* canvas;
	LinearLayout *layout;
	void setElementCallbacks();

	Vector<SettingElement*> elements;
	int cursor;

};

}
#endif