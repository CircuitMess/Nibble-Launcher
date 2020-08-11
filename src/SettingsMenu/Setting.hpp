#ifndef SETTINGSTRUCT_H
#define SETTINGSTRUCT_H
#include <Arduino.h>
#include <Util/Vector.h>
namespace SettingsMenu
{
struct Setting
{
	enum Type { NUMERIC, BOOLEAN } type;
	void* params = nullptr;
	std::string label;
	void* storeLocation = nullptr;
	Setting(Type type, void* params, std::string label, void* storeLocation) :
			type(type), params(params), label(label), storeLocation(storeLocation){}
};

struct SettingValueList
{
	Vector<int> values;
	SettingValueList(Vector<int> values) : values(values){}
};
}

#endif