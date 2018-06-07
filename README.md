# HW-DC26-Badge
This is the project page for the Hacker Warehouse electronic badge that was made for DEF CON 26.

![](https://files.hackerwarehouse.com/HW-DC26-Badge/badge-r1.jpg)

## Badge Hardware Specs
* ESP32
* Atmel 32u4
* 96×64 Graphic Full Color OLED Display
* 14x Mini NeoPixel RGB LEDs
* 4 tactile buttons arranged in Joypad configuration
* 8 Mb Storage
* 2x2 "[Shitty Add-on](https://hackaday.io/project/52950-defcon-26-shitty-add-ons "Shitty Add-on")" support
* Powered via 2 AA batteries or USB

## Badge Functions
* HID Arsenal
	* Ducky script compatible payloads (Windows and Linux demo payloads included)
	* Expanded ducky commands to support mouse movement and clicks
	* Static strings (yubico like)
	* Mouse jiggler (anti screensaver)
	* Time-based One-Time Password and HMAC-based One-Time Password (Google Authenticator mode /w keyboard insertion on demand)
* WiFi Tools
	* 802.11 channel activity monitor
	* Access point scanner / Open AP detector
	* Packet Monitor and Deauth sniffer
	* Can act as a client or access point
	* Webserver (edit filesystem files, trigger payloads remotely)
	* NTP client
* LED patterns and OLED graphics
* Misc. extras

## Usage Instructions
### Buttons / UI
![](https://files.hackerwarehouse.com/HW-DC26-Badge/button-labels.jpg)
Up and Down is on the left side of the OLED, and Left and Right is on the right side of the OLED. From left most to right:  Down / Up / Left / Right

In order to go into a menu or run a program, click the right button. To exit most applications, click the left button. Some programs like the snake game require all 4 buttons so there is no way to clean exit, a power cycle is needed. 

### Keyboard and Mouse Payloads
When the badge is connected to a computer through the USB port, it can act as a HID device. The most common uses of that device class is keyboards and mice. Through use of the Ducky language and expanding on it to support mouse movement, the badge can trigger these scripts on demand. 

#### Via Hardware Menu
Navigate to the HID Arsenal menu, then select either the Windows or Linux Payloads menu option, depending on your target device. In the next submenu, there are five slots for payloads and pressing the right button triggers them. The slots are configurable via win.json and lin.json which defines the payload name and file location.

#### Via Webserver
Assuming the Webserver is running (via Server Tools -> Webserver Start), navigate to 10.0.5.1. Authenticate via the credentials on the OLED. You can remotely trigger the stored payloads or write on demand payloads via this interface. The stored payloads read the same win.json and lin.json files. 

### Static Strings
This mode is something like the yubico device. It is simply a static string that you can utilize with another password. This is a method of two factor authentication as the static string would be likely long and complex. Disclaimer: There is no security mechanism in place if someone were to obtain the hardware and this isn't a FIPS certified device.

### Mouse Jiggler
This mode is utilized if you have a device that you don't want the screen saver to be triggered. The mouse will move every few seconds in a defined pattern. This is useful if you have a device that forces a screensaver or device lock after x idle time. 

### Google Authenticator
This mode requires the badge to be connected to an access point that has internet connectivity so it can reach a time server as there is no RTC on the device itself. In the WiFi client tools menu, connect to an AP and make sure NTP is running. From there, in the HID Arsenal menu select the 'Google Auth (TOTP)' submenu. The six digit time based unique code will refresh every seconds. Press the right button to insert that set of digits automatically assuming the badge is connected through USB. 

### Webserver
Navigate to WiFi Server Tools -> Webserver Start. You can access the webserver if running an AP or if connected. The pages are protected through basic authentication, username and password generated dynamically and is provided on the screen.

### Blinky
There are 40+ LED patterns included. We've included our favorites set in a 5 second rotation. You can also trigger each type of pattern individually. 

### Other Modes
There are no hidden modes, but there are a few areas we didn't cover in this set of docs. Explore. 

## Configuration / Customization
### Settings menu in OLED UI
After feedback from last year on brightness settings, there are individual settings for OLED and LED brightness. Region setting is related to WiFi channel scanning.

### JSON configuration files
There are 4 different json files in SPIFFS. There are a couple ways to edit these files and get them loaded on the badge, but the easiest is through the webserver.
1. config.json = Main device configuration. Includes parameters such as LED and OLED brightness settings, ssid/password if connecting to a wireless client network, region information, and google auth HMAC. 
2. lin.json / win.json = Configuration files for the Linux and Windows KB/M payloads. 
3. static.json = Configuration file for the stored static strings. 

### Google Authenticator
If you do not have a 2 factor code from Google already, follow these steps to set obtain one and change your security settings: 
https://myaccount.google.com/security -> Signing into Google -> 2 Factor Authentication -> Authenticator App -> iphone -> click the 'Can't scan it' button and you will get a string of text that looks like this (and remember that your code will be different than this example):

![](https://files.hackerwarehouse.com/HW-DC26-Badge/google-authenticator-set-223911.png)


Take that text, remove all the spaces, change the letter casing to all upper case, then 
input the text into config.json





## Compiling Instructions
These instructions will get you a copy of the project up and running on your local machine.
### Prerequisites
What things you need to install the software and how to install them
```
1. Arduino / compiler
2. SparkFun Pro Micro v3 com driver
https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/installing-windows
https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/installing-mac--linux
3. Libraries
  https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json
  https://github.com/adafruit/Adafruit_NeoPixel
  https://github.com/adafruit/Adafruit-GFX-Library
  https://github.com/adafruit/Adafruit_CircuitPlayground
  https://github.com/kitesurfer1404/WS2812FX
  https://github.com/me-no-dev/ESPAsyncWebServer
  https://github.com/me-no-dev/AsyncTCP
  https://github.com/bblanchon/ArduinoJson
  https://github.com/me-no-dev/arduino-esp32fs-plugin
  Manually copy the SSD_13XX and qMenuSystem folders to Arduino libraries folder. We included a snapshot of the other libraries used in case newer versions break our code.
4. 3.3v FTDI serial cable
5. and of course a Hacker Warehouse DC26 Badge
```
### Compiling / Flashing Atmel Code
* Set the board to SparkFun Pro Micro
* Set the processor as ATmega32u4 3.3v and NOT the 5v one

### Compiling / Flashing ESP Code
* Set the board to SparkFun ESP32 Thing

#### Programming through FTDI port
1. Flash the atmel with the 'atmel-esp32ftdi-pin7' code first, as this toggle pin 7 and the rx/tx switch to change the serial port communiction to the FTDI port
2. Connect FTDI cable to the board while holding the left button down (which is on the right side, top button)
3. Upload code. Speed setting doesn't really matter, we've been using 921600.
4. To put the atmel back to its original state, program 'atmel-km-relay' the code to it.

#### Programming through USB port / Atmel
Pass through programming doesn't work at the time of writing. Communication is passed through, but something is holding up the ESP32 from fully going into the programming mode

### Uploading SPIFFS files
The keyboard/mouse payloads, configuration files, webserver files are all files that are loaded into a special section in storage marked as SPIFFS. While still having pin 7 set to allow for the FTDI connection, use the 'ESP32 Sketch Data Upload' function in the tools menu. This will compress and compile all the files in the 'data' folder, and upload it. 

## Expanding on your own
### Adding to menu
* See files HW-DC26-Badge.ino and mainmenu.h
* Look at the main switch statement in the .ino file
* Look any of the mainmenu.h examples for how it corresponds to the display

### Button mapping / Escape code
* See files HW-DC26-Badge.ino and blinky.cpp 
* Button mapping is in the beginning of the .ino file
* Look at the LED_Flashlight function for an example of button usage and how to quit the loop and go back to main menu

## Background / timeline of badge development
* Second custom hardware / electronics project trying to improve upon the things we learned from last year
	* Changed from the ESP8266 to the newer ESP32 so we could leverage prior code and focus on enhancements 
	* Added an LDO regulator so badge or connected computer doesn't freak out on voltage
* 3 hardware revisions = 2 prototype runs = 4-5 months
	* The hardware revisions this year were minor in comparison to last year. It was mainly due to the 2x2 Shitty Add-on and orientation of the connector
	* Tried to start earlier (lesson learned last year) but scheduling conflicts put us back to essentially the same timeline as last year
* 5 week code sprint before code needing to be frozen for mass badge flashing
	* This was a massive improvement from last year from a time / rush perspective
	* Still wanted to add more into it such as BLE, but ram out of storage and ram. 

## Authors / Contributors
* **Garrett Gee**

## Acknowledgments
* Hat tip to anyone who's code was used, looked at, referenced, etc
* All the supporters of this project
