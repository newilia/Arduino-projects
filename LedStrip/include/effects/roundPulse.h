#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "swordGeometry.h"
#include "utils.h"


float getBrightnessFactor(float pointRadius, float circleRadius)
{
	const float circleWidth = 2;

	float radiusDiff = fabs(pointRadius - circleRadius);

	if (radiusDiff > circleWidth)
	{
		return 0;
	}
	
	return cos(radiusDiff * M_PI / circleWidth / 2);
}

void roundPulse()
{
	static float circleRadius;
	static float circleBrightness;
	static uint8_t hue = 5;
	static float speed = 0.3;
	const float speedMult = 0.98; 
	const float brightnessIncr = 10;
	static Pair<float, float> center = {4.5, 8};

	EVERY_N_MILLIS_I(pulseReset, g_settings.roundPulse.frameTime * 55 + 500)
	{
		pulseReset.setPeriod(g_settings.roundPulse.frameTime * 55 + 500);
		circleRadius = 0;
		circleBrightness = 0;
		hue += g_settings.roundPulse.hueStep;
		
		speed = 0.3;
	}

	EVERY_N_MILLIS_I(pulseTimer, g_settings.roundPulse.frameTime)
	{
		pulseTimer.setPeriod(g_settings.roundPulse.frameTime);
		circleRadius += speed;
		speed *= speedMult;
		circleBrightness = min(255, circleBrightness + brightnessIncr);

		for (int i = 0; i < LEDS_COUNT; ++i)
		{
			auto ledPos = Sword::GetLedPosition(i);
			ledPos.first -= center.first;
			ledPos.second -= center.second;
			auto ledRadius = sqrt(ledPos.first * ledPos.first + ledPos.second * ledPos.second);

			if (g_settings.roundPulse.isGlow)
			{
				float brightnessFactor = getBrightnessFactor(ledRadius, circleRadius);
				CRGB color = CHSV(hue, 255, brightnessFactor * circleBrightness);
				g_leds[i] = utils::MixColors(color, g_leds[i], brightnessFactor);
			}
			else
			{
				float brightnessFactor = getBrightnessFactor(ledRadius, circleRadius);
				g_leds[i] = CHSV(hue, 255, brightnessFactor * circleBrightness);
			}
		}
		
		FastLED.show();
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<uint8_t>(BUTT_UP, BUTT_DOWN, g_settings.roundPulse.frameTime, 5, 50, 5);
		utils::IrControlValue<uint8_t>(BUTT_LEFT, BUTT_RIGHT, g_settings.roundPulse.hueStep, 0, 255, 10);
		utils::IrControlBool(BUTT_OK, g_settings.roundPulse.isGlow);
	}
}