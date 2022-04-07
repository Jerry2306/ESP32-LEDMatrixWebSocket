#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <FastLED.h>

#include "networkCredentials.h"

#define LEDCount 256

const char *ssid = SSID;
const char *password = PASSWORD;

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

CRGB leds[LEDCount];

#include "html_page.h"
#include "functions.h"

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  server.on("/", webpage);

  FastLED.addLeds<WS2812B, 2, GRB>(leds, LEDCount);
  FastLED.setBrightness(10);
  FastLED.show();

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
}