#ifndef NIBBLE_NUMERICSETTINGELEMENT_H
#define NIBBLE_NUMERICSETTINGELEMENT_H
#include "SettingElement.h"
#include "../Setting.hpp"
namespace SettingsMenu
{
class NumericSettingElement : public SettingElement
{
public:
	NumericSettingElement(ElementContainer* parent, Setting* setting);
	void draw();
	void pressLeft() override;
	void pressRight() override;
	void pressSelect() override;
private:
	int numSteps;
	Vector<int> values;
	int currentStep;
	std::string parseSeconds(uint seconds);

};
} // namespace Settings


#endif