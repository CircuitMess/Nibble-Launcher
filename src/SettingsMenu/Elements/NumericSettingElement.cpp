#include "NumericSettingElement.h"
#include <Arduino.h>
#include <string>
#include <iostream>
#include <sstream>
Settings::NumericSettingElement::NumericSettingElement(ElementContainer* parent, Setting* setting) :
		SettingElement(parent, setting)
{
	values = ((SettingValueList*)setting->params)->values;
	numSteps = values.size();
	currentStep = 0;
	blinkTime = 0;
	active = 0;
}
void Settings::NumericSettingElement::draw()
{
	getSprite()->setCursor(getTotalX() + 2, getTotalY());
	getSprite()->setTextColor(TFT_BLACK);
	getSprite()->setTextSize(1);
	getSprite()->setTextFont(2);
	getSprite()->setTextWrap(0);
	getSprite()->print(setting->label.c_str());
	getSprite()->print(": ");
	getSprite()->print(parseSeconds(values[currentStep]).c_str());
	if(blinkToggle)
	{
		getSprite()->setCursor(getTotalX() + 2, getTotalY() + 18);
		getSprite()->print(parseSeconds(values[0]).c_str());
		getSprite()->setCursor(getTotalX() + getWidth() - 25, getTotalY() + 18);
		getSprite()->print(parseSeconds(values[values.size() - 1]).c_str());
	}

	getSprite()->drawRect(getTotalX() + 28, getTotalY() + 22, getWidth() - 56, 7, TFT_BLACK);
	getSprite()->fillRect(getTotalX() + 30, getTotalY() + 24, map(currentStep, 0, numSteps - 1, 0, getWidth() - 60), 3, TFT_BLACK);
	Element::draw();
}
std::string Settings::NumericSettingElement::parseSeconds(uint seconds)
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
void Settings::NumericSettingElement::pressLeft()
{
	currentStep = max(0, currentStep - 1);
}
void Settings::NumericSettingElement::pressRight()
{
	currentStep = min(numSteps - 1, currentStep + 1);
}
void Settings::NumericSettingElement::pressSelect()
{
}
void* Settings::NumericSettingElement::getSelection()
{
	return &values[currentStep];
}