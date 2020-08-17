cmake_minimum_required(VERSION 3.15)
project(Nibble-Launcher)

set(CMAKE_CXX_STANDARD 14)

include_directories(~/.arduino15/packages/esp8266/hardware/esp8266/2.7.1/cores/esp8266)

include_directories(~/Arduino/libraries/CircuitOS/src)
include_directories(~/Arduino/libraries/TFT_eSPI)
include_directories(~/Arduino/libraries/TFT_eSPI/Extensions)
include_directories(~/Arduino/libraries/Nibble/src)

AUX_SOURCE_DIRECTORY(src SOURCES)
AUX_SOURCE_DIRECTORY(src/Elements SOURCES)
AUX_SOURCE_DIRECTORY(src/SettingsMenu SOURCES)
AUX_SOURCE_DIRECTORY(src/SettingsMenu/Elements SOURCES)
AUX_SOURCE_DIRECTORY(src/Services SOURCES)

add_definitions(-DDEBUG)
add_definitions(-DCIRCUITOS_LITTLEFS)
add_definitions(-DCIRCUITOS_TONE)
add_definitions(-DCIRCUITOS_LOWRAM)

SET_SOURCE_FILES_PROPERTIES(Nibble-Launcher.ino PROPERTIES LANGUAGE CXX)
add_executable(Nibble-Launcher ${SOURCES} Nibble-Launcher.ino)