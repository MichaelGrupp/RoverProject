# RoverProject
Projects with the DFROBOT Pirate 4WD and an Arduino Leonardo compatible board (ROMEO v2.0), plus several sensors.

~under construction~

![Current set-up of the Rover](https://github.com/MichaelGrupp/RoverProject/blob/master/doc/rover.JPG)

##Software Development
* This project is being developed in Visual Studio 2015 with the Visual Micro plugin, but the source code can be also used with the Arduino suite or any other compatible IDE/compiler of your choice.
* C++ is the main language used for the libraries, with raw C (or even assembler) only used where really needed.
    * concepts of the C++11 standard (e.g. `auto` or `enum class`) are used to keep the code as nice and modern as possible
    * **You have to specify C++11 compatibility manually** - to do so, go to your Arduino installation folder and open `hardware/arduino/avr/platform.txt`. Append `-std=gnu++11` to the cpp compiler flags and reboot the IDE:

`...`

`compiler.cpp.flags=-c -g -Os {compiler.warning_flags} -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -std=gnu++11`

`...`

##Usage
Simply copy the folders of the libraries you want to use in your project (e.g. `libDrive`) to the libraries path of your Arduino IDE (on Windows: `Documents/Arduino/libraries`) and use them like any other Arduino library.

## Hardware
**Mobile Platform:** DFROBOT Pirate 4WD  
(http://www.dfrobot.com/index.php?route=product/product&path=37_111&product_id=97&sort=p.price&order=ASC)

**Microcontroller Board:** DFROBOT Romeo v2.0 (Arduino Leonardo compatible)  
(http://www.dfrobot.com/index.php?route=product/product&path=35_104&product_id=844)

**Infrared Range Sensor:** Sharp 2Y0A21 (analog)  
(http://www.sharpsma.com/webfm_send/1489)

**Ultrasonic Range Sensor:** HC-SR04  
(http://www.micropik.com/PDF/HCSR04.pdf)

**Wheel Encoder:** DFROBOT SEN0038  
(http://www.dfrobot.com/index.php?route=product/product&path=36_68&product_id=98)

**Binary Light Sensor:** Grove Line Finder  
(http://www.seeedstudio.com/wiki/Grove_-_Line_Finder)

**LED Bar:** Grove LED Bar  
(http://www.seeedstudio.com/wiki/Grove_-_LED_Bar)

