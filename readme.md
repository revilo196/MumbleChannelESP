# MumbleChannelESP

This Project implements the mumble control message protocol on an ESP32. This makes it possible for a ESP32 to connect to Mumble Server and collect information about channels, users and more. With some adaptions it could also be possible to send messages and more.

The main goal of this project was to implement a notification LED that blinks when the usercount in a channel reaches a threshold. I chould not find any libraries or examples that do this.

The library I implemented is mainly focused on user and channel data, but could easily be expanded to new tasks.

## Usage

Change the *config.h* to your needs, compile, upload and enjoy your channel notification light.

In the *config.h* you need to configure WIFI credentials, Mumble Server, username, channelname and the threshold for the blinking led.

## Important Note
This client uses an insecure TLS connection, and makes no certificate checks, this makes it easy to connect to local servers. But it's insecure, be warned! This insecure connection is only avaiable in the newest ArduinoESP32 version. That could differ from the PlatformIO version and may needs to be updated manually. 

## Architachture

The Sofware is split in two parts, the arduino based connection that sends and receives messages, and a high-level implementation for message handling and userdatabase. 

This high-level is desiged to be tested using native tests.

The high-level implementation is desigend to be good to understand, and is not optimized for performance.

## Dependencies

The mumble contol message protocol is based on Googles protocol-buffers.
I used the embedded protobuff implementation nanopb. To rebuild the .c/.h for the protocol-buffers(e.g. when you changed some options) you need to regenerate using the nanopb protoc version (this is not incuded in the platfromIO dependencies). See the origial nanopb repo (https://github.com/nanopb/nanopb)

## Limitations
Most of the mumble contol messages dont define a field max size for strings and other types. This is bad for nanobp, makes parsing messages much more complicated. So I defined some maxinmum sizes for some fields using the *mumble.optons* file. Changes in this file need a pb rebuild as descibed above. This could also lead to errors if e.g. a username exeeds the defined limit.
