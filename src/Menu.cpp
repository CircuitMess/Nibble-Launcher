#include "Menu.h"

#include <Nibble.h>
#include <Display/Sprite.h>
#include <Support/Context.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Audio/Piezo.h>
#include "SettingsMenu/SettingsStruct.hpp"
#include "Services/BatteryService.h"

Menu* Menu::instance = nullptr;

Menu::Menu(Display& display) : canvas(display.getBaseSprite()), screen(display, 100, 100),
	layout(&screen, VERTICAL), audioLayout(&layout, HORIZONTAL), exit(&layout, screen.getWidth() - 10, 20),
	muteText(&audioLayout, screen.getWidth() - 60 - 10, 20), audioSwitch(&audioLayout){

	instance = this;
	screen.setPos(128, 128);
	buildUI();
}

void Menu::toggle(Context* currentContext){
	if(shown) stop();
	else start(currentContext);
}

void Menu::start(Context* currentContext){
	this->currentContext = currentContext;

	shown = true;
	shownDone = false;
	stopping = false;
	selectElement(0);
	audioSwitch.set(settings()->audio, true);
	if(showProgress == 0){
		currentContext->stop();
		showProgress = 0.00001f;
		LoopManager::addListener(this);
	}
}

void Menu::stop(bool immediate){
	Settings::store();

	if(immediate){
		stopping = true;
		LoopManager::removeListener(this);

		shown = false;
		showProgress = 0;
		exiting = false;
		currentContext = nullptr;

		releaseInput();
		return;
	}


	if(showProgress == 1){
		showProgress = 0.99999f;
	}

	shown = false;
}

void Menu::bindInput(){
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->stop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		if(instance->selectedElement == 0){
			instance->audioSwitch.toggle();
			settings()->audio = instance->audioSwitch.getState();
			Piezo.setMute(!settings()->audio);
			Piezo.tone(500, 50);
		}else{
			instance->exiting = true;
			instance->stop();
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance == nullptr) return;
		instance->selectElement(instance->selectedElement == 0 ? 1 : 0);
		Piezo.tone(500, 50);
	});

	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance == nullptr) return;
		instance->selectElement((instance->selectedElement + 1) % 2);
		Piezo.tone(500, 50);
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		if(instance->selectedElement == 0){
			settings()->audio = 1;
			Piezo.setMute(!settings()->audio);
			if(instance->audioSwitch.getState() == false)
			{
				Piezo.tone(500, 50);
			}
			instance->audioSwitch.set(true);
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		if(instance->selectedElement == 0){
			instance->audioSwitch.set(false);
			settings()->audio = 0;
			Piezo.setMute(!settings()->audio);
			Piezo.tone(500, 50);
		}
	});
}

void Menu::releaseInput(){
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
}

void Menu::selectElement(uint8_t index){
	layout.reposChildren();
	audioLayout.reposChildren();
	selectedElement = index;

	exit.setColor(TFT_WHITE);
	muteText.setColor(TFT_WHITE);

	TextElement* selectedText = selectedElement == 0 ? &muteText : &exit;
	selectAccum = 0;
	selectedX = selectedText->getX();
	selectedText->setColor(TFT_YELLOW);
}

void Menu::draw(){
	currentContext->draw();
	canvas->fillRect(screen.getX(), screen.getY(), layout.getWidth(), layout.getHeight(), TFT_DARKGREY);
	canvas->drawRect(screen.getX(), screen.getY(), layout.getWidth(), layout.getHeight(), TFT_LIGHTGREY);
	canvas->drawRect(screen.getX() + 1, screen.getY() + 1, layout.getWidth() - 2, layout.getHeight() - 2, TFT_LIGHTGREY);
	screen.draw();
	screen.commit();
}

void Menu::loop(uint micros){
	if(stopping) return;

	if(showProgress != 0 && showProgress != 1){
		if(shown){
			showProgress = min(1.0f, showProgress + (float) micros / 1000000.0f * 5.0f);
		}else{
			showProgress = max(0.0f, showProgress - (float) micros / 1000000.0f * 5.0f);
		}

		uint y = (canvas->height() - layout.getHeight()) / 2;
		screen.setPos(14, y + (1.0f - showProgress) * (canvas->height() - y + 15));
	}else{
		selectAccum += (float) micros / 1000000.0f;

		TextElement* selectedText = selectedElement == 0 ? &muteText : &exit;
		int8_t newX = selectedX + sin(selectAccum * 5.0f) * 3.0f;
		selectedText->setX(newX);
	}

	if(!BatteryService::getInstance()->modalShown()){
		draw();
	}

	if(showProgress == 0 || showProgress == 1){
		if(showProgress == 0){
			releaseInput();
			LoopManager::removeListener(this);

			if(exiting){
				currentContext->pop();
				exiting = false;
			}else{
				currentContext->start();
			}

			currentContext = nullptr;
		}else if(showProgress == 1 && !shownDone){
			shownDone = true;
			bindInput();
		}
	}
}

void Menu::buildUI(){
	layout.setWHType(PARENT, CHILDREN);
	layout.setPadding(5);
	layout.setGutter(5);
	layout.reflow();

	audioLayout.setWHType(PARENT, CHILDREN);
	audioLayout.addChild(&muteText);
	audioLayout.addChild(&audioSwitch);
	audioLayout.reflow();

	layout.addChild(&audioLayout);
	layout.addChild(&exit);

	layout.reflow();
	screen.addChild(&layout);
	screen.repos();

	muteText.setText("Sound");
	muteText.setFont(1);
	muteText.setSize(1);
	muteText.setColor(TFT_WHITE);

	exit.setText("Exit Game");
	exit.setFont(1);
	exit.setSize(1);
	exit.setColor(TFT_WHITE);
	exit.setAlignment(TextElement::CENTER);
}
