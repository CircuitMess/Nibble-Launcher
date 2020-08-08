#ifndef NIBBLE_SLIDER_H
#define NIBBLE_SLIDER_H,
#include <UI/CustomElement.h>
#include "../SettingStruct.hpp"
namespace Settings
{
class NumericSettingElement : public CustomElement
{
public:
	NumericSettingElement(ElementContainer* parent, Setting* setting);
	void draw();
private:
	Setting* setting;
	uint numSteps;
	Vector<int> values;
	std::string parseSeconds(uint seconds);
};
} // namespace Settings


#endif