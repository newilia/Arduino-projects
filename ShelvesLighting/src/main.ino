/*
	Скетч разработан 30.11.2018 Wirekraken
*/
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <FastLED.h>
#include "GlobalDefs.h"
#include "ServerUtils.h"
#include "Effects.h"

// инициализация websocket на 81 порту
WebSocketsServer webSocket(81);
ESP8266WebServer server(80);
CRGBArray<LEDS_COUNT> g_leds;

void setup()
{
	Serial.begin(115200);
	FastLED.setBrightness(255);
	FastLED.addLeds<WS2811, LEDS_PIN, GRB>(g_leds, LEDS_COUNT);
	FastLED.showColor(CRGB::Black);

	WiFi.config(Ip, Gateway, Subnet);
	WiFi.begin(ssid, password);

	Serial.println("Connecting...");

	Effects::FadingColor onConnectingEffect;
	while (WiFi.status() == WL_DISCONNECTED)
	{
		onConnectingEffect.Show();
		delay(10);
	}
	FastLED.showColor(CRGB::Black);

	if (!WiFi.isConnected())
	{
		Serial.println("Connection failed: code " + String((int)WiFi.status()));
		for (int i = 0; i < (int)WiFi.status(); ++i)
		{
			FastLED.showColor(CRGB::Black);
			delay(300);
			FastLED.showColor(CRGB::Red);
			delay(300);
		}
		FastLED.showColor(CRGB::Black);
		return;
	}
	
	
	Serial.print("Connected. IP address: ");
	Serial.println(WiFi.localIP());

	server.onNotFound([]()
	{
		if (!handleFileRead(server, server.uri()))
		{
			server.send(404, "text/plain", String("FileNotFound") + server.uri());
		}
	});
	
	server.begin();
	SPIFFS.begin();	
	webSocket.begin();
	webSocket.onEvent(webSocketEvent);
}

void loop()
{
	webSocket.loop();
	server.handleClient();
	ledEffect(0);
}


// функция эффектов
void ledEffect(int ledMode)
{ 
	switch(ledMode)
	{
		
	}
}