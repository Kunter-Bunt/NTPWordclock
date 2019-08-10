# NTPWordclock
This project hosts an Arduino Sketch for controlling a wordclock. 
It relies on the Network Time Protocol (NTP) to get the current time.

## Goals of this project
The project was initialized because a lot of the code for the numerous wordclock tutorials out there was of poor quality, 
lacking extensibility and often required much hardware.

This project utilizes NTP to get the time rather than relying on a radio-clock. 
Additionally by using smart LED strips the amount of hardware required reduces to only one ESP8266-Module 
(I've used a Wemos D1 Mini) and a NeoPixel-like LED Strip (for me, a cheap one with a WS2812B-Chipset).
  
## Requirements for this project
- Esp8266-Development-Module with USB-Port (e.g. WeMos D1 Mini)
- USB-Power-Supply + cable
- Strip of LEDs with a Chipset supported by [FastLED](https://github.com/FastLED/FastLED)
- Basic Soldering Equipment to connect the Esp8266 to the LED strip and parts of the LED strips
- A WiFi network that always has a connection to the internet when you want to use the clock
- The non technical materials of your wordclock tutorial
  
## Project files
### Wordclock.ino
Hosts the most important configurations, Setup and Loop methods.

### LEDControl.ino
Controls the LED strip(s).
Hook in here if you have a different layout than me (western german).
The LEDControl hosts "Word-Functions" which are combined to a sentence displayed on your clock.

### NTPControl.ino
Sends requests to a timeserver and sets the clock internal time.

### WifiControl.ino
Connects to WiFi and hosts a very simple RESTServer to be able to control the clock. 
Since requests only get handled once per Interval, response is very slow.
