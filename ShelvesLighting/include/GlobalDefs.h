#pragma once
#include <Arduino.h>

#define DEBUG 0
#define time_t uint32_t

const int LEDS_COUNT = 72;
const int LEDS_PIN = 2;

const char* ssid = "138"; 
const char* password = "88888888"; 

IPAddress Ip(192,168,0,123); // IP-адрес для ESP
IPAddress Gateway(192,168,0,1); // IP-адрес шлюза (роутера)
IPAddress Subnet(255,255,255,0); // маска подсети, диапазон IP-адресов в локальной сети
 