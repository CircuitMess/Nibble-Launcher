#include "MainMenu.h"
#include "Invaderz/SpaceInvaders.h"
#include <Input/Input.h>
MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), gamesMenu(&screen, 3){

	instance = this;
	menuItems.push_back({new SpaceInvaders(display), new BitmapElement(&gamesMenu, SpaceInvaders::getIcon(), 40, 40)});
	buildUI();
	pack();
}

void MainMenu::start(){
	Serial.println("start");
	delay(5);
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		Serial.println("push");
		delay(5);
		Context* app = instance->menuItems[instance->gamesMenu.getSelected()].game;
		if(app == nullptr) return;
		
		app->push(instance);
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		Serial.println("LEFT PRESS");
		delay(5);
		instance->gamesMenu.selectPrev();
		instance->screen.commit();
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		Serial.println("RIGHT PRESS");
		delay(5);
		instance->gamesMenu.selectNext();
		instance->screen.commit();
	});

	draw();
}

void MainMenu::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
}

void MainMenu::unpack(){
	Context::unpack();

	for(auto& item : menuItems){
		item.image->getSprite()->clear(TFT_GREEN);
	}
}

void MainMenu::draw(){
	// Serial.println("draw");
	// delay(5);
	screen.draw();
	screen.commit();
	Serial.println("draw done");
	delay(5);
}

void MainMenu::fillMenu(){
	// Serial.println("fill menu");
	for(auto& item : menuItems){
		Serial.println(item.game->getTitle());
		gamesMenu.addItem({ item.game->getTitle(), item.image });
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
