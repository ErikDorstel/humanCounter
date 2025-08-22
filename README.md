# humanCounter
ESP32 based human counter for the fire department
#### Features
* uses two RC522
* uses one LED traffic light
* uses two light barriers
* implements a WiFi AP and WiFi Client
* sends updates to a server part on tcp port 4242
* implements a tcp server on port 4242
* implements a telnet server on port 23
#### RC522 A
* GPIO 19 MISO
* GPIO 23 MOSI
* GPIO 18 SCLK
* GPIO 5 CS
* GPIO 34 IRQ
* GPIO 4 RST
#### RC522 B
* GPIO 12 MISO
* GPIO 13 MOSI
* GPIO 14 SCLK
* GPIO 15 CS
* GPIO 35 IRQ
* GPIO 33 RST
#### LED traffic light
* GPIO 27 beep
* GPIO 32 green
* GPIO 25 yellow
* GPIO 26 red
#### Light barrier
* GPIO 36 A
* GPIO 39 B