#include "SettingsMenu.h"
#include "SettingStruct.hpp"
#include "Elements/NumericSettingElement.h"
#include "Elements/BoolSettingElement.h"


Vector<Setting> settings = {
	Setting(Setting::Type::NUMERIC, new SettingValueList(Vector<int>{0, 10, 30, 60, 300}), std::string("Sleep")),
	Setting(Setting::Type::NUMERIC, new SettingValueList(Vector<int>{0, 300, 600, 1800, 3600}), std::string("Shutdown")),
	Setting(Setting::Type::BOOLEAN, nullptr, std::string("Audio"))
};

Settings::SettingsMenu* Settings::SettingsMenu::instance = nullptr;

Settings::SettingsMenu::SettingsMenu(Display& display) :
		Context(display), display(&display), canvas(display.getBaseSprite()),
		layout(LinearLayout(&screen, VERTICAL))
{
	instance = this;

	layout.setWHType(PARENT, PARENT);
	layout.setPadding(3);
	layout.setGutter(8);
	layout.reflow();
	screen.addChild(&layout);
	for (uint8_t i = 0; i < settings.size(); i++)
	{
		Settings::SettingElement* element;
		switch (settings[i].type)
		{
		case Setting::Type::NUMERIC:
			element = new Settings::NumericSettingElement(&layout, &settings[i]);
			break;
		case Setting::Type::BOOLEAN:
			element = new Settings::BoolSettingElement(&layout, &settings[i]);
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
void Settings::SettingsMenu::draw()
{
	canvas->clear(TFT_LIGHTGREY);
	screen.draw();
}
void Settings::SettingsMenu::update(uint _time)
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
void Settings::SettingsMenu::start()
{
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
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
		if(instance->cursor < settings.size() - 1)
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
void Settings::SettingsMenu::stop()
{
	UpdateManager::removeListener(this);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
}
void Settings::SettingsMenu::pack()
{
	
}
void Settings::SettingsMenu::setElementCallbacks()
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