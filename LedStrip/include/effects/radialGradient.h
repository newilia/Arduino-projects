#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "swordGeometry.h"
#include "utils.h"


void radialGradient()
{
	EVERY_N_MILLIS(20)
	{
		float ms = millis();
		const Pair<float, float> center = {4.5, 7};

		for (int i = 0; i < LEDS_COUNT; ++i)
		{
			auto ledPos = Sword::GetLedPosition(i);
			ledPos.first -= center.first;
			ledPos.second -= center.second;
			auto ledRadius = sqrt(ledPos.first * ledPos.first + ledPos.second * ledPos.second);
			
			float hue = g_settings.radialGradient.speed * (g_settings.radialGradient.dirOut ? -ms : ms);
			hue += ledRadius * g_settings.radialGradient.scale;
			g_leds[i] = CHSV(hue, 255, g_settings.radialGradient.brightness);
		}
		
		FastLED.show();
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<float>(BUTT_DOWN, BUTT_UP, g_settings.radialGradient.speed, 0.03, 0.3, 0.03);
		utils::IrControlValue<float>(BUTT_LEFT, BUTT_RIGHT, g_settings.radialGradient.scale, 2, 40, 3);
		utils::IrControlValue<uint8_t>(BUTT_STAR, BUTT_HASH, g_settings.radialGradient.brightness, 32, 255, 15);
		utils::IrControlBool(BUTT_OK, g_settings.radialGradient.dirOut);
	}
}