#ifndef NIBBLE_BOOLSETTINGELEMENT_H
#define NIBBLE_BOOLSETTINGELEMENT_H,
#include "SettingElement.h"
#include "../Setting.hpp"
namespace SettingsMenu
{
class BoolSettingElement : public SettingElement
{
public:
	BoolSettingElement(ElementContainer* parent, Setting* setting);
	void draw();
	void pressLeft() override;
	void pressRight() override;
	void pressSelect() override;
private:
	bool state;

};
} // namespace Settings


#endif