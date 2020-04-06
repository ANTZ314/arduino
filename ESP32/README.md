# ESP32 WiFi Module:

**INstall ESP32:**

* File -> Preferences -> Additional Board Manager URLs
	https://dl.espressif.com/dl/package_esp32_index.json
* Tools -> Board -> Boards Manager
* search "ESP32" -> (ESP32 by Espressif Systems) -> Install



### Project Folders:

* **ESP32_ADC** - Test the ADC input of the ESP32
* **ESP32_Blink** - Simple ESP32 onboard LED blink test
* **ESP32_FREQ1** - PWM to output a frequency on the "" pin
* **ESP32_GPIO** - GPIO test program
* **ESP32_HallSens** - Hall Effect Sensor test program
* **ESP32_OLED** - Basic OLED testing of various output functions
* **ESP32_OLED1** - Basic OLED testing of various output functions
* **ESP32_OLED2** - Basic OLED testing of various output functions
* **ESP32_OLED3** - Basic integration of of MRF522 RFID reader & OLED96W
* **ESP32_OLED_Demo** - Graphic library demo for the ESP32
* **ESP32_OLED_RFID** - Integration of the MRF522 RFID reader, OLED and OLED's graphics library for ESP32.
* **ESP32_RFID** - ESP32 and MRF522 RFID reader. Output result to Serial Monitor.

IP: assigned dynamically and gotten from "SerialMonitor" at run-time

* **SimpleWebServerWiFi** - hosts the server from the module
* **SoftAP1** - Original WiFi test code (hosts the server from the module) - **NOT CONNECTING**
* **SoftAP2** External device connects to ESP32 WiFi host. Simple control HTML with 2 buttons for switching & table for data display - **NOT CONNECTING**
* **SoftAP3** - Added ADC input to serial & then to HTML update - **NOT CONNECTING**
* **SoftAP4** - 2 Button Web Server Example - WORKING. Switches ESP32 output pins from Web Server Buttons.


**ADC Links:**

* [IoT ESP32 Instractables](http://www.instructables.com/id/IOT-Made-Simple-Playing-With-the-ESP32-on-Arduino-/)
* [ESP32 SAR ADC](https://esp-idf.readthedocs.io/en/v2.0/api/peripherals/adc.html)
* [AnalogReadResolution](https://www.arduino.cc/en/Reference.AnalogReadResolution)

**OLED Links**

[INSTRUCTABLES](https://www.instructables.com/id/The-First-Usage-of-096-I2C-OLED-Display-With-Ardui/)
[MICRO-ROBOTICS](https://www.robotics.org.za/OLED096W)

**How to add WiFi Control to any project || ESP32 Beginner's Guide**  
[**GreatScott LINK**](https://www.youtube.com/watch?v=ZAqNKaX3LQ0)

* [MIT App Inventor](http://appinventor.mit.edu/explore/front.html)  
* [Linux Install](http://appinventor.mit.edu/explore/ai2/linux.html)  
* [Designer](http://appinventor.mit.edu/explore/designer-blocks.html)  
* [Tutorial](http://appinventor.mit.edu/explore/ai2/hellopurr.html)

------------------------------
### [ESP32](http://esp32.net/)

[ESPRESSIF](https://www.espressif.com/en/esp-wroom-32/resources)  
[GET STARTED](https://esp-idf.readthedocs.io/en/latest/get-started/index.html#start-a-project)  
[PROGRAMMING GUIDE](https://esp-idf.readthedocs.io/en/latest/index.html)  
[GITHUB - AURDUINO (C++) PROJECT](https://github.com/espressif/arduino-esp32)

-------
## HTML:

[SPIFFS](http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html#file-system-object-spiffs)  
[Web Server with HTML 1](https://circuits4you.com/2016/12/16/esp8266-web-server-html/)  
[Web Server with HTML 2](https://circuits4you.com/2018/02/03/esp8266-nodemcu-adc-analog-value-on-dial-gauge/)  
[IOT based home automation](https://circuits4you.com/2016/05/19/iot-based-home-automation-project/)  
[Gauge Example 1](https://github.com/sathomas/material-gauge)  
[Gauge Example 2](https://gist.github.com/sathomas/a8f1cc13c8b4fcb63c6f)

------------------------
## Arduino Installation:  
* Install latest Arduino IDE from arduino.cc

* Open Terminal and execute the following command (copy->paste and hit enter):

        sudo usermod -a -G dialout $USER && \
        sudo apt-get install git && \
        wget https://bootstrap.pypa.io/get-pip.py && \
        sudo python get-pip.py && \
        sudo pip install pyserial && \
        mkdir -p ~/Arduino/hardware/espressif && \
        cd ~/Arduino/hardware/espressif && \
        git clone https://github.com/espressif/arduino-esp32.git esp32 && \
        cd esp32 && \
        git submodule update --init --recursive && \
        cd tools && \
        python2 get.py

Remember Permissions:  

    sudo chmod a+rw /dev/ttyUSB0
    
**TROUBLE SHOOTING**
If there are no ESP32 modules listed in the tools -> boards list:  

	sudo apt remove --purge arduino  
	
Download from [ARDUINO.CC](https://www.arduino.cc/en/Main/Software) and install from 'tar.xz' file.
