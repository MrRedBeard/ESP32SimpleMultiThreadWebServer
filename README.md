# ESP32MultiProcessWebServer
Simple multithreaded Web Server that utilizes 2 cores of the ESP32 separating the web server from the additional executable tasks. Using xTaskCreatePinnedToCore and global variables I can kick off a seperate thread from my main loop running the webserver. 

Derived from multi process loops by https://github.com/copercini
https://github.com/copercini/esp32-iot-examples/blob/master/multiloop/multiloop.ino

Derived from the ESP32 webserver sample

LED is hooked to pin 12
![alt text](https://github.com/MrRedBeard/ESP32MultiProcessWebServer/raw/master/Board.gif)

Very simple Web Interface
![alt text](https://github.com/MrRedBeard/ESP32MultiProcessWebServer/raw/master/WebInterface.PNG)
