#include "BoolSettingElement.h"
#include <Arduino.h>
#include <string>
#include <iostream>
#include <sstream>
SettingsMenu::BoolSettingElement::BoolSettingElement(ElementContainer* parent, Setting* setting) :
		SettingElement(parent, setting)
{
	state = 0;
	if(setting->storeLocation != nullptr)
	{
		state = *(bool*)(setting->storeLocation);
	}
}
void SettingsMenu::BoolSettingElement::draw()
{
	getSprite()->setCursor(getTotalX() + 2, getTotalY() - 1);
	getSprite()->setTextColor(TFT_BLACK);
	getSprite()->setTextSize(1);
	getSprite()->setTextFont(2);
	getSprite()->setTextWrap(0);
	getSprite()->print(setting->label.c_str());
	getSprite()->print(":");
	getSprite()->setCursor(getTotalX() + 31, getTotalY() + 14);
	getSprite()->print("ON");
	getSprite()->setCursor(getTotalX() + 74, getTotalY() + 14);
	getSprite()->print("OFF");
	if(state && (!active || (active && blinkToggle)))
	{
		getSprite()->drawRect(getTotalX() + 22, getTotalY() + 15, 34, 14, TFT_BLACK);
	}
	else if(!state && (!active || (active && blinkToggle)))
	{
		getSprite()->drawRect(getTotalX() + 66, getTotalY() + 15, 40, 14, TFT_BLACK);
	}
	Element::draw();
}
void SettingsMenu::BoolSettingElement::pressLeft()
{
	if(!state){
		state = 1;
		*((bool*)setting->storeLocation) = state;
		blinkToggle = 1;
		blinkTime = 0;
	}
}
void SettingsMenu::BoolSettingElement::pressRight()
{
	if(state){
		state = 0;
		*((bool*)setting->storeLocation) = state;
		blinkToggle = 1;
		blinkTime = 0;
	}
		
}
void SettingsMenu::BoolSettingElement::pressSelect()
{
}