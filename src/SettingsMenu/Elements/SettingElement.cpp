#include "SettingElement.h"
Settings::SettingElement::SettingElement(ElementContainer* parent, Setting* setting) : 
		CustomElement(parent, parent->getAvailableWidth(), 30), setting(setting)
{
	blinkTime = 0;
	active = 0;
	blinkToggle = 1;
}

Settings::SettingElement::~SettingElement()
{
}
void Settings::SettingElement::setActive(bool _active)
{
	blinkTime = 0;
	if(_active)
	{
		active = 1;
		blinkToggle = 0;
	}
	else
	{
		active = 0;
		blinkToggle = 1;
	}
}
void Settings::SettingElement::updateBlink(uint _time)
{
	if(active)
	{
		blinkTime+=_time;
		if(blinkTime >= 350000)
		{
			blinkTime = 0;
			blinkToggle = !blinkToggle;
		}
	}
}
void Settings::SettingElement::update(uint _time)
{
	updateBlink(_time);
}
bool Settings::SettingElement::isActive()
{
	return active;
}