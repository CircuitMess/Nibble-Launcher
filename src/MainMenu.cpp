#include "MainMenu.h"
#include <Input/Input.h>
#include <Display/Color.h>
#include <UI/BitmapElement.h>
#include "Invaderz/icon.h"

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), gamesMenu(&screen, 3){

	instance = this;
	menuItems.push_back({ "Invaderz", new BitmapElement(&gamesMenu, icon, 24, 26) });
	buildUI();
	pack();
}

#define BTN_A 32
#define BTN_B 27
#define BTN_LEFT 34
#define BTN_RIGHT 33

void MainMenu::start(){
	Serial.println("start");
	delay(5);


	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		instance->gamesMenu.selectPrev();
		instance->screen.commit();
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		instance->gamesMenu.selectNext();
		instance->screen.commit();
	});
	screen.getSprite()->clear(TFT_BLACK);
	draw();
}

void MainMenu::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
}

void MainMenu::draw(){
	screen.draw();
	screen.commit();
}

void MainMenu::fillMenu(){
	for(auto& item : menuItems){
		gamesMenu.addItem({ item.title, item.image });
	}
}

void MainMenu::buildUI(){
	Serial.println("buildUI");
	fillMenu();

	gamesMenu.setWHType(PARENT, PARENT);
	gamesMenu.setTitleColor(TFT_RED, TFT_BLACK);
	gamesMenu.reflow();

	screen.addChild(&gamesMenu);
	screen.repos();


}
