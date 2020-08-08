#include "SettingsMenu.h"
#include "SettingStruct.hpp"
#include "Elements/NumericSettingElement.h"
Vector<Setting> settings = {
	Setting(Setting::Type::NUMERIC, static_cast<void*>(Vector<int>{0, 10, 30, 60, 300, 600, 1800, 3600}.data()), std::string("Sleep time"))
};

SettingsMenu::SettingsMenu* SettingsMenu::SettingsMenu::instance = nullptr;

SettingsMenu::SettingsMenu::SettingsMenu(Display& display) :
		Context(display), display(&display), canvas(display.getBaseSprite()),
		layout(LinearLayout(&screen, VERTICAL))
{
	instance = this;
	for (uint8_t i = 0; i < settings.size(); i++)
	{
		switch (settings[i].type)
		{
		case Setting::Type::NUMERIC:
			layout.addChild(new Settings::NumericSettingElement(&layout, &settings[i]));
			break;
		
		default:
			break;
		}
	}
}
void SettingsMenu::SettingsMenu::draw()
{
	canvas->clear(TFT_LIGHTGREY);
	screen.draw();
}
void SettingsMenu::SettingsMenu::update(uint _time)
{
	draw();
	display->commit();
}
void SettingsMenu::SettingsMenu::start()
{
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		instance->pop();
	});
}
void SettingsMenu::SettingsMenu::stop()
{
	Input::getInstance()->removeBtnPressCallback(BTN_B);
}
void SettingsMenu::SettingsMenu::pack()
{
	
}