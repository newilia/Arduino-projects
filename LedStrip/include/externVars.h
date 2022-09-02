#pragma once
#include <FastLED.h>
#include "globalDefs.h"
//#include <GyverEncoder.h>
#include "settings.h"
#include "IrControl.h"

extern CRGB g_leds[LEDS_COUNT];
//extern Encoder g_enc1;
extern time_t g_currentMs;
extern time_t g_lastFrameMs;
extern Settings g_settings;
extern IrControl g_ir;