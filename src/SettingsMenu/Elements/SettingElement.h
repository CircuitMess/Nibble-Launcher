#ifndef NIBBLE_SETTINGELEMENT_H
#define NIBBLE_SETTINGELEMENT_H
#include <UI/CustomElement.h>
#include "../SettingStruct.hpp"

namespace Settings
{

class SettingElement : public CustomElement
{
public:
	SettingElement(ElementContainer* parent, Setting* setting);
	~SettingElement();
	virtual void* getSelection() = 0;
	virtual void setActive(bool _active);
	virtual void update(uint _time);
	virtual void pressLeft() = 0;
	virtual void pressRight() = 0;
	virtual void pressSelect() = 0;
	bool isActive();

protected:
	Setting* setting;
	
	void updateBlink(uint _time);
	uint blinkTime;
	bool active;
	bool blinkToggle;
};

} // namespace Settings

#endif