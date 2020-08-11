#include "NumericSettingElement.h"
#include <Arduino.h>
#include <string>
#include <iostream>
#include <sstream>
#include "../SettingsStruct.hpp"
SettingsMenu::NumericSettingElement::NumericSettingElement(ElementContainer* parent, Setting* setting) :
		SettingElement(parent, setting)
{
	values = ((NumericSettingParams*)setting->params)->values;
	numSteps = values.size();
	currentStep = 0;
	if(setting->storeLocation != nullptr && values.indexOf(*((int*)setting->storeLocation)) != 0xFFFFFFFF)
	{
		currentStep = max((uint)0, values.indexOf(*((int*)setting->storeLocation)));
	}
	blinkTime = 0;
	active = 0;
}
void SettingsMenu::NumericSettingElement::draw()
{
	getSprite()->setCursor(getTotalX() + 2, getTotalY() - 1);
	getSprite()->setTextColor(TFT_BLACK);
	getSprite()->setTextSize(1);
	getSprite()->setTextFont(2);
	getSprite()->setTextWrap(0);
	getSprite()->print(setting->label.c_str());
	getSprite()->print(": ");
	getSprite()->print(parseSeconds(values[currentStep]).c_str());
	if(blinkToggle)
	{
		getSprite()->setCursor(getTotalX() + 2, getTotalY() + 15);
		getSprite()->print(parseSeconds(values[0]).c_str());
		getSprite()->setCursor(getTotalX() + getWidth() - 25, getTotalY() + 15);
		getSprite()->print(parseSeconds(values[values.size() - 1]).c_str());
	}

	getSprite()->drawRect(getTotalX() + 28, getTotalY() + 19, getWidth() - 56, 7, TFT_BLACK);
	getSprite()->fillRect(getTotalX() + 30, getTotalY() + 21, map(currentStep, 0, numSteps - 1, 0, getWidth() - 60), 3, TFT_BLACK);
	Element::draw();
}
std::string SettingsMenu::NumericSettingElement::parseSeconds(uint seconds)
{
	std::ostringstream str1;
	if(seconds >= 3600)
	{
		str1 << uint(seconds/3600) << "h";
	}
	else if(seconds >= 60)
	{
		str1 << uint(seconds/60) << "min";
	}
	else
	{
		str1 << seconds << "s";
	}
	return str1.str();
}
void SettingsMenu::NumericSettingElement::pressLeft()
{
	currentStep = max(0, currentStep - 1);
	*((int*)setting->storeLocation) = values[currentStep];
}
void SettingsMenu::NumericSettingElement::pressRight()
{
	currentStep = min(numSteps - 1, currentStep + 1);
	*((int*)setting->storeLocation) = values[currentStep];
}
void SettingsMenu::NumericSettingElement::pressSelect()
{
}