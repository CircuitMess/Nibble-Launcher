#ifndef NIBBLE_BOOLSETTINGELEMENT_H
#define NIBBLE_BOOLSETTINGELEMENT_H,
#include "SettingElement.h"
#include "../SettingStruct.hpp"
namespace Settings
{
class BoolSettingElement : public SettingElement
{
public:
	BoolSettingElement(ElementContainer* parent, Setting* setting);
	void draw();
	void pressLeft() override;
	void pressRight() override;
	void pressSelect() override;
	void* getSelection() override;
private:
	bool state;

};
} // namespace Settings


#endif