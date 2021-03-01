#ifndef CONFIG_H
#define CONFIG_H

// Replace with your actual SSID and password:
#define WIFI_SSID "Your SSID here"
#define WIFI_PASSWD "WLAN AP password here"
#define MUMBLE_SERVER "Server IP here"
#define MUMBLE_PORT  64738

#define CLIENT_NAME "ESP32"  // name of the client when connecting

#define CHANNEL_NAME "Root"  // name of the channel to look for (not joining just looking)
#define CHANNEL_COUNT_THESHHOLD 2  // amount of clients connected to CHANNEL_NAME when blink starts

#define LED_PIN 12

#endif
