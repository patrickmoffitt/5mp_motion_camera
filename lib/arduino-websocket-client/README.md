## Websocket Client for Arduino

This is a simple library that implements a Websocket client running on an Arduino. This instance is a fork of https://github.com/wblommaert/arduino-websocket-client

### Getting started

Install the library to "libraries" folder in your Arduino sketchbook folder. For example, on a Mac that's `~/Documents/Arduino/libraries`, while on Windows it can be found under `C:/Users/%username%/Documents/Arduino/libraries`.

Try the examples to ensure that things work.

Start playing with your own code! I will be adding my own implementation to the examples soon, which provides a toggle for WiFi and Ethernet clients, depending on which one you're using. The original library only supported the EthernetClient.

### Notes

This library has been enhanced and supports messages larger than 65535 characters. Do NOT try this on an Arduino Uno though, as that platform has been reported to fail using such large messages. In addition, this library supports single-frame text frames, continuation frames, binary frames, and ping/pong frames.

### Websocket Server

The Websocket server that is included in this repository is the original version by [brandenhall](https://github.com/brandenhall/Arduino-Websocket), which in turn is based on [ejeklint](https://github.com/ejeklint/ArduinoWebsocketServer). All credit goes to them as I have no plans to further improve this for now.

### Credits

Thank you to Github user [ejeklint](https://github.com/ejeklint) for the excellent starting point for this library. From his original Hixie76-only code [Branden](https://github.com/brandenhall) was able to add support for RFC 6455 and create the WebSocket client. I enabled the support for messages larger than 65535 characters based off Javascript and PHP code. Note that this won't work on the Arduino Uno due to platform restrictions.
