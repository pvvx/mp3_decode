# ESP8266 MP3 webradio streaming player 
---
Support all ESP2866 modules (512 Kbytes flash)

Free Flash (512 Kbytes): 200 Kbytes.
Free Heap: 20 Kbytes.

GPIO3 (RXD) - Output PWM Sound

MP3-decodec-out uses IRAM Buffer > 20 Kbytes

Set connect AP in playerconfig.h: 

define AP_NAME "HOMEAP"

define AP_PASS "01234567890"

The base code is at the official Espressif Github: 
https://github.com/espressif/esp8266_mp3_decoder

UDK compiler: 
http://esp8266.ru/forum/forums/devkit/