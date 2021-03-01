#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>


#include "mumble.h"
#include "mumble_data.h"
#include "mumble_channelcounter.h"


#include "config.h"
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWD;
const char* host = MUMBLE_SERVER;
const int port = MUMBLE_PORT;
const char * channelName = CHANNEL_NAME;
const int channelWarnThres = CHANNEL_COUNT_THESHHOLD;

const uint8_t led = LED_PIN;

uint32_t last_ping = 0;

WiFiClientSecure client; // TLS connections
MumbleConnection mumble(host,port, CLIENT_NAME ,&client); // Mumble Control Send and Receive
MumbleData m_data(&mumble); // Mumble User and Channel database
MumbleChannelThreasholdProvider watcher(&m_data, &m_data, channelName, channelWarnThres); // keep tack of a specific channel count

bool blink = false;
void blink_on() {blink = true; Serial.println("BLINK LED ON");}
void blink_off() {blink = false; Serial.println("BLINK LED OFF");}

EventListenerFunc onListener(&blink_on);  //Listen on MumbleChannelThreasholdProvider events 
EventListenerFunc offListener(&blink_off);  //Listen on MumbleChannelThreasholdProvider events 

void wifi_connect(const char* _ssid  , const char* _password);

void setup()
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  wifi_connect(ssid, password);

  mumble.connect();
  watcher.connect();
  watcher.add_Listener(&onListener, MumbleChannelThreasholdProvider::OVEREQU);
  watcher.add_Listener(&offListener, MumbleChannelThreasholdProvider::UNDER);

  last_ping = millis();
}


void loop() {
  
  //send ping every 10 sek to keep connection running (must be atleast 30 sek)
  if(millis() > last_ping+10000) {
    mumble.send_ping();
    last_ping = millis();
  }


  for (size_t i = 0; i < 10; i++)
  {

    if(i<5 && blink) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }


    delay(100);

    //polling on new available messages
    mumble.read_available();
  }

}


void wifi_connect(const char* _ssid  , const char* _password) {
    Serial.print("Connecting to ");
    Serial.println(_ssid);

    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    pinMode(led, OUTPUT);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}