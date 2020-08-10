#ifndef NIBBLE_MENU_H
#define NIBBLE_MENU_H

#include <Display/Display.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <UI/TextElement.h>
#include <Update/UpdateListener.h>
#include "Elements/Switch.h"

class Sprite;
class Context;

class Menu : public UpdateListener {
public:
	Menu(Display& display);

	void update(uint micros) override;
	void draw();

	void start(Context* currentContext);
	void stop();
	void toggle(Context* currentContext);

private:
	static Menu* instance;

	Sprite* canvas;
	Context* currentContext = nullptr;

	Screen screen;
	LinearLayout layout;
	TextElement exit;
	LinearLayout muteLayout;
	TextElement muteText;
	Switch muteSwitch;

	float showProgress = 0;
	bool shown = false;

	bool exiting = false;

	float selectAccum = 0;
	uint8_t selectedX;
	uint8_t selectedElement = 0;
	void selectElement(uint8_t index);

	void buildUI();
	void bindInput();
	void releaseInput();
};


#endif //NIBBLE_MENU_H
