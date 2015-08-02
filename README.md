# ESP8266 MP3 webradio streaming player 
---
Do not use external components.<br> 
Module ESP-01 + resistor and capacitor:<br>
https://www.youtube.com/watch?v=XIneZEfahmQ<br>

GPIO3 (RXD) - Output PWM/Sigma-Delta Sound<br>
MP3-decodec uses IRAM Buffer > 20 Kbytes<br>

Support all ESP2866 modules (0.5..16 Mbytes flash)<br>

Set connect AP in playerconfig.h: 
define AP_NAME "HOMEAP"<br>
define AP_PASS "01234567890"<br>

The base code is at the [official Espressif Github](https://github.com/espressif/esp8266_mp3_decoder)<br>

[RTOS SDK ver 1.0.4](https://github.com/espressif/esp_iot_rtos_sdk)<br>

[MAD: MPEG Audio Decoder](http://www.underbit.com/products/mad/)<br>

[UDK compiler](http://esp8266.ru/forum/forums/devkit/)<br>