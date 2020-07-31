#include "Launcher.h"
#include "Nibble.hpp"
#include <Input/Input.h>

#include "Splash.h"
#include "GameScroller.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "Games/Bonk/GameInfo.hpp"
#include "Games/SpaceRocks/GameInfo.hpp"
#include "Games/Snake/GameInfo.hpp"
#include "Games/Invaderz/GameInfo.hpp"
#include "../GameInfo.hpp"
#include "BatteryService/BatteryService.h"
#include "Bitmaps/battery.hpp"

const GameInfo games[] = {
	InvaderzInfo, BonkInfo, SpaceRocksInfo, SnakeInfo
};

Launcher* instance = nullptr;

Launcher::Launcher(Display* display, BatteryService* batteryService) : Context(*display), display(display), canvas(display->getBaseSprite()),
		scroller(new GameScroller(canvas, games)), logo(new Logo(canvas)), title(new GameTitle(canvas)), batteryService(batteryService)
{
	instance = this;
	canvas->setChroma(TFT_TRANSPARENT);

	splash = new Splash(display->getBaseSprite(), logo, title, scroller);
}

void Launcher::start(){
	runningContext = this;
	if(splash == nullptr){
		bindInput();
	}
	UpdateManager::addListener(this);
}

void Launcher::stop()
{
	UpdateManager::removeListener(this);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
}

void Launcher::prev(){
	uint8_t selecting = instance->scroller->prev();
	if(selecting != selectedGame){
		instance->title->change(games[selecting].title);
	}
	selectedGame = selecting;
}

void Launcher::next(){
	uint8_t selecting = instance->scroller->next();
	if(selecting != selectedGame){
		instance->title->change(games[selecting].title);
	}
	selectedGame = selecting;
}

void Launcher::bindInput(){
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		instance->next();
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		instance->prev();
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance->scroller->scrolling()) return;

		Display* display = instance->display;
		uint8_t index = instance->selectedGame;

		Context* game = games[index].launch(*display);
		game->push(instance);
		runningContext = game;
	});
}

void Launcher::update(uint micros){
	if(splash){
		splash->update(micros);

		if(splash->done()){
			delete splash;
			splash = nullptr;

			bindInput();
			scroller->splash(1);
			title->change(games[selectedGame].title);
		}
	}else{
		logo->update(micros);
	}

	draw();
	screen.commit();
}

void Launcher::draw(){
	canvas->clear(TFT_BLACK);
	scroller->draw();
	title->draw();
	logo->draw();

	if(batteryService->getVoltage() > 780)
	{
		canvas->drawMonochromeIcon(battery1, 120, 0, 8, 12, 1, TFT_WHITE);
	}
	else if(batteryService->getVoltage() <= 780 && batteryService->getVoltage() >= 700)
	{
		canvas->drawMonochromeIcon(battery2, 120, 0, 8, 12, 1, TFT_WHITE);
	}
	else if(batteryService->getVoltage() < 680)
	{
		canvas->drawMonochromeIcon(battery3, 120, 0, 8, 12, 1, TFT_WHITE);
	}
}
