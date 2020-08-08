#include "NumericSettingElement.h"
Settings::NumericSettingElement::NumericSettingElement(ElementContainer* parent, Setting* setting) :
		CustomElement(parent, parent->getAvailableWidth(), 30), setting(setting),
		values(*static_cast<Vector<int>*>(setting->params)),
		numSteps(values.size())
{

}
void Settings::NumericSettingElement::draw()
{
	getSprite()->setCursor(getTotalX() + 2, getTotalY());
	getSprite()->setTextColor(TFT_BLACK);
	getSprite()->setTextSize(2);
	getSprite()->print(setting->label.c_str());
	getSprite()->setCursor(getTotalX() + 2, getTotalY() + 18);
	getSprite()->print(parseSeconds(values[0]).c_str());
	getSprite()->setCursor(getTotalX() - 20, getTotalY() + 18);
	getSprite()->print(parseSeconds(values[values.size() - 1]).c_str());
	Element::draw();
}
std::string Settings::NumericSettingElement::parseSeconds(uint seconds)
{
	if(seconds >= 3600)
	{
		return std::string(uint(seconds/3600) + "hrs");
	}
	if(seconds >= 60)
	{
		return std::string(uint(seconds/60) + "min");
	}
	return std::string(seconds + "s");
}