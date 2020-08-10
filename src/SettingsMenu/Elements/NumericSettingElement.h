#ifndef NIBBLE_SLIDER_H
#define NIBBLE_SLIDER_H,
#include "SettingElement.h"
#include "../SettingStruct.hpp"
namespace Settings
{
class NumericSettingElement : public SettingElement
{
public:
	NumericSettingElement(ElementContainer* parent, Setting* setting);
	void draw();
	void pressLeft() override;
	void pressRight() override;
	void pressSelect() override;
	void* getSelection() override;
private:
	int numSteps;
	Vector<int> values;
	int currentStep;
	std::string parseSeconds(uint seconds);

};
} // namespace Settings


#endif