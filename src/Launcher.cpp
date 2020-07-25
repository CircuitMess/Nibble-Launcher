#include "Launcher.h"
#include "Nibble.hpp"
#include <Input/Input.h>

#include "GameScroller.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "Games/Bonk/GameInfo.hpp"
#include "Games/SpaceRocks/GameInfo.hpp"
#include "Games/Snake/GameInfo.hpp"
#include "Games/Invaderz/GameInfo.hpp"
#include "../GameInfo.hpp"

const GameInfo games[] = {
	InvaderzInfo, BonkInfo, SpaceRocksInfo, SnakeInfo
};

Launcher* instance = nullptr;

Launcher::Launcher(Display* display) : Context(*display), display(display), canvas(display->getBaseSprite()),
		scroller(new GameScroller(canvas, games)), logo(new Logo(canvas)), title(new GameTitle(canvas)){

	instance = this;
	canvas->setChroma(TFT_TRANSPARENT);
	title->change(games[selectedGame].title);
}

void Launcher::start()
{
	runningContext = this;
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
		// delete instance;

		Context* game = games[index].launch(*display);
		game->push(instance);
		game->draw();

		Sprite* canvas = display->getBaseSprite();
		for(int i = 128; i >= 0; i -= 2){
			canvas->setPos(0, i);
			display->commit();
		}

		game->start();
		runningContext = game;
	});

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

void Launcher::update(uint micros){
	draw();
	screen.commit();
}

void Launcher::draw(){
	canvas->clear(TFT_BLACK);
	logo->draw();
	scroller->draw();
	title->draw();
}
