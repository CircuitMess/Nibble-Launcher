#ifndef BYTEBOY_MAINMENU_H
#define BYTEBOY_MAINMENU_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Elements/GridMenu.h>
#include <UI/BitmapElement.h>
#include "ByteBoy.hpp"

struct LauncherItem {
	Game* game;
	Element* image;
};

class MainMenu : public Context {
public:
	MainMenu(Display& display);

	void start() override;
	void stop() override;
	void unpack() override;
	void draw() override;

private:
	static MainMenu* instance;

	GridMenu gamesMenu;
	std::vector<LauncherItem> menuItems;

	void fillMenu();
	void buildUI();

};


#endif
