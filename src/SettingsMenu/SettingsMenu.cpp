#include "SettingsMenu.h"
#include "Setting.hpp"
#include "Elements/NumericSettingElement.h"
#include "Elements/BoolSettingElement.h"
#include "SettingsStruct.hpp"


Vector<SettingsMenu::Setting> settingsVector = {};

SettingsMenu::SettingsMenu* SettingsMenu::SettingsMenu::instance = nullptr;

SettingsMenu::SettingsMenu::SettingsMenu(Display& display) :
		Context(display), display(&display), canvas(display.getBaseSprite()),
		layout(LinearLayout(&screen, VERTICAL))
{
	instance = this;

	settingsVector = {
		Setting(Setting::Type::NUMERIC, new SettingValueList(Vector<int>{0, 10, 30, 60, 300}), std::string("Sleep"), &(settings()->sleepTime)),
		Setting(Setting::Type::NUMERIC, new SettingValueList(Vector<int>{0, 300, 600, 1800, 3600}), std::string("Shutdown"), &(settings()->shutdownTime)),
		Setting(Setting::Type::BOOLEAN, nullptr, std::string("Audio"), &(settings()->audio))
	};

	layout.setWHType(PARENT, PARENT);
	layout.setPadding(3);
	layout.setGutter(8);
	layout.reflow();
	screen.addChild(&layout);
	for (uint8_t i = 0; i < settingsVector.size(); i++)
	{
		SettingElement* element;
		switch (settingsVector[i].type)
		{
		case Setting::Type::NUMERIC:
			element = new NumericSettingElement(&layout, &settingsVector[i]);
			break;
		case Setting::Type::BOOLEAN:
			element = new BoolSettingElement(&layout, &settingsVector[i]);
			break;
		default:
			break;
		}
		elements.push_back(element);
		layout.addChild(element);
	}
	layout.reposChildren();
	cursor = 0;
	elements[0]->setActive(1);
}
void SettingsMenu::SettingsMenu::draw()
{
	canvas->clear(TFT_LIGHTGREY);
	screen.draw();
}
void SettingsMenu::SettingsMenu::update(uint _time)
{
	for (uint8_t i = 0; i < elements.size(); i++)
	{
		if(elements[i]->isActive())
		{
			elements[i]->update(_time);
		}
	}
	draw();
	display->commit();
}
void SettingsMenu::SettingsMenu::start()
{
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		Settings::store();
		instance->pop();
	});
	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance->cursor > 0)
		{
			instance->elements[instance->cursor]->setActive(0);
			instance->cursor--;
			instance->elements[instance->cursor]->setActive(1);
			instance->setElementCallbacks();
		}
	});
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance->cursor < settingsVector.size() - 1)
		{
			instance->elements[instance->cursor]->setActive(0);
			instance->cursor++;
			instance->elements[instance->cursor]->setActive(1);
			instance->setElementCallbacks();
		}
	});
	setElementCallbacks();
	UpdateManager::addListener(this);
}
void SettingsMenu::SettingsMenu::stop()
{
	UpdateManager::removeListener(this);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
}
void SettingsMenu::SettingsMenu::pack()
{
	
}
void SettingsMenu::SettingsMenu::setElementCallbacks()
{
	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		instance->elements[instance->cursor]->pressLeft();
	});
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		instance->elements[instance->cursor]->pressRight();
	});
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		instance->elements[instance->cursor]->pressSelect();
	});
}