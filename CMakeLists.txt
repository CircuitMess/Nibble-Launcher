cmake_minimum_required(VERSION 3.15)
project(Nibble)

set(CMAKE_CXX_STANDARD 14)

include_directories(~/.arduino15/packages/esp8266/hardware/esp8266/2.7.1/cores/esp8266)

include_directories(~/Arduino/libraries/CircuitOS/src)
include_directories(~/Arduino/libraries/TFT_eSPI)
include_directories(~/Arduino/libraries/TFT_eSPI/Extensions)

AUX_SOURCE_DIRECTORY(src SOURCES)
AUX_SOURCE_DIRECTORY(src/BatteryService SOURCES)
AUX_SOURCE_DIRECTORY(src/SleepService SOURCES)
AUX_SOURCE_DIRECTORY(src/Elements SOURCES)

add_definitions(-DDEBUG)

SET_SOURCE_FILES_PROPERTIES(ByteBoi.ino PROPERTIES LANGUAGE CXX)
add_executable(Nibble ${SOURCES} Nibble.ino)