# MumbleChannelESP

This Projects implements the mumble contol message protocol on an ESP32. This makes it possible for a ESP32 to connect to Mumble Server and collect inforation about channels, users and more. With some Adoption it could also be possible to send Messages and more.

The main goal of the project was to implement a notification LED that binks when the usercount in a channel reaches a threashold. I chould not find any libaries or examples that do this.

The Libary I implemented is mainly focused on user and channel data but could easyly expanded to new tasks.

## Usage

change the *config.h* to your needs, compile, upload and enjoy your channel notification light

In the *config.h* you need to configre WIFI crendetials, Mumble Server, username, channelname and the threshold for the blink led,

## Important Note
This client uses an insecure TLS connection, and makes no certificate checks, this makes it easy to connect to local servers. But is insecure be warned! This Insecure connection is only availble in the newest ArduinoESP32 version. That could differ from the PlatformIO version. May needs to be updated manualy 

## Architachture

The Sofware is split in two parts, the arduino based conmnection that send and receives messages, and a high-level implementation for message handleing and userdatabase. 

This high-level is desiged to be teseted using native tests.

The high-level implementation is desigend to be good to understand, and is not optimized for performance.

## Dependencies

The mumble contol message protocol is based on googles protocol-buffers.
I used the embedded protobuff implementation nanopb. To rebuild the .c/.h for the protocol-buffers(e.g. when you changed some options) you need regenerate using the nanopb protoc version (this is not incuded in the platfromIO dependencie) see the origial nanopb repo (https://github.com/nanopb/nanopb)

## Limitations
Most of the mumble contol messages dont define a field max size for strings and other types. this is bad for nanobp, makes parsing messages much more complicated. So I defied some maxinmum sizes for some felds. using the *mumble.optons* file. changes in this file needs a pb rebuild descibed above. This could also lead to errors if e.g. a username exeeds the defind limit.