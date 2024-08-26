
# ARDUINO README:
-----------------

#### Libraries:
* ESP8266 and ESP32 Oled Driver for SSD1306 display
* ESP32
* Blynk
* AddicoreRFID
* Adafruit SSD1306
* Adafruit GFX Library


**Permission:**  

Check the port in Tools & Get USB permissions in Linux:  

	sudo chmod a+rw /dev/ttyACM0
	sudo chmod a+rw /dev/ttyACM1
	sudo chmod a+rw /dev/ttyUSB0
	sudo chmod a+rw /dev/ttyUSB1

**OLED Screen Size**
/home/antz/Arduino/libraries/Adafruit_SSD1306/Adafruit_SSD1306.h
Comment other sizes - line 97 - 110

### Project List:

* **blinktest** - Blink 4 LED's consecutively (UNO?)

* **BookCase**:
	* BookCase1 - LED Fading sketch applied to LED strips
	* BookCase2a - Added Button & debounce
	* BookCase2b - Adapted '2a' to Arduino Micro
* **Clock1** - Digital Winding Clock (LCD type?)

* **ESP32**:
	* **ESP32_ADC** - ADC input
	* **ESP32_Blink** - Blink LED on selected output pin
	* **ESP32_FREQ1** - Generate PWM frequency (square/sin wave?)
	* **ESP32_GPIO** - ? ? ? test
	* **ESP32_HallSens** - Hall Effect Sensor test. Output value to serial monitor
	* **ESP32_OLED** - ? ? test
	* **ESP32_OLED1** - seems like unfinished OLED test
	* **ESP32_OLED2** - SImple counter displayed on OLED096W
	* **ESP32_OLED3** - RFID Reader (SPI) with OLED (I2C) combined functionality
	* **ESP32_RFID** - RC522 RFID Reader toggles various GPIO according to ID being read.
	* **SimpleServer** - Basic WiFi server example. Can connect and transfer data
	* **SoftAP1** - Simple web application example displaying data being sent from the ESP32
	* **SoftAP2** - Simple web application example displaying data being sent from the ESP32
	* **SoftAP3** - Simple web application example displaying data being sent from the ESP32
	
* **LCD2X16** - This sketch prints "Hello World!" to the 2x16 LCD

* **OLED096W** -

* **RFID:**
	* **RFID_1** - Example to read the ID from an RC522
	* **RFID_2** - Example to read the ID from an RC522 (basically the same)
	* **RFID_ESD** - Read the RFID tag and blink 'LOCKED' or 'UNLOCKED' led as indicator action
	* **RFID_LCD1** - Combination of the LCD hello world second counter example and the Standard Addicore_RFID_Example code 
	
* **RGB** - 

* **Serial_Monitor1** - 
