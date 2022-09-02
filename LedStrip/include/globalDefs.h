#pragma once
#include <Arduino.h>

const int LEDS_COUNT = 45 + 4;
const int FIRST_LED_OFFSET = 32;
const int LEDS_PIN = 2;

const int ENC1_S1_PIN = 4;
const int ENC1_S2_PIN = 5;
const int ENC1_KEY_PIN = 6;

const int IR_PIN = 3;

const int VOLTAGE_PIN = A6;

const float DISCHARGE_VOLTAGE = 3.1;
const float LOW_VOLTAGE = 3.5;
const float HIGH_VOLTAGE = 4.2;

#define time_t uint32_t
#define REMOTE_TYPE 1 //wavgat

#define FASTLED_ALLOW_INTERRUPTS 1 //don't really know why, but... let it be