cmake_minimum_required(VERSION 3.15)
project(ByteBoi-Launcher)

set(CMAKE_CXX_STANDARD 14)

include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/Wire/src)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/FS/src)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/SD/src)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/SPI/src)

include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/cores/esp32)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk/include)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk/include/esp32)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk/include/freertos)
include_directories(~/Arduino/libraries/NeoPixelBus/src)
include_directories(~/Arduino/libraries/CircuitOS/src)

AUX_SOURCE_DIRECTORY(src SOURCES)
AUX_SOURCE_DIRECTORY(src/Invaderz SOURCES)

add_definitions(-DDEBUG)

SET_SOURCE_FILES_PROPERTIES(ByteBoy.ino PROPERTIES LANGUAGE CXX)
add_executable(ByteBoi-Launcher ${SOURCES} ByteBoy.ino)