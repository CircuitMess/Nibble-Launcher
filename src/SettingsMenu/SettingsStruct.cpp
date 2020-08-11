#include "SettingsStruct.hpp"
SettingsStruct* settings(){
	return static_cast<SettingsStruct*>(Settings::data());
}
