#ifndef SETTINGSTRUCT_H
#define SETTINGSTRUCT_H
#include <Arduino.h>

struct Setting
{
	enum Type { NUMERIC, BOOLEAN } type;
	void* params = nullptr;
	std::string label;
	Setting(Type type, void* params, std::string label) :
			type(type), params(params), label(label){}
};


#endif