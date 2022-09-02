#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "utils.h"

void fadeall()
{
	for (int i = 0; i < LEDS_COUNT; i++)
	{
		g_leds[i].nscale8(250);
	}
};

void setPixel(int i, uint8_t hue)
{
	g_leds[i] = CHSV(hue, 255, 255);
	FastLED.show();
	// now that we've shown the leds, reset the i'th led to black
	// leds[i] = CRGB::Black;
	fadeall();
	// Wait a little bit before we loop around and do it again
};

void cylon()
{
	EVERY_N_MILLIS_I(cyclonTimer, g_settings.cyclon.frameTime)
	{
		cyclonTimer.setPeriod(g_settings.cyclon.frameTime);

		static int i = 0;
		static bool dirRight = true;
		static float hue = 0;

		setPixel(i, hue);

		hue += g_settings.cyclon.hueIncrease;
		hue = ((uint8_t)hue) % 256;

		dirRight ? ++i : --i;

		if ((dirRight && i == LEDS_COUNT - 1) || (!dirRight && i == 0))
		{
			if (g_settings.cyclon.reflect)
			{
				dirRight = !dirRight;
			}
			else
			{
				i = dirRight ? 0 : LEDS_COUNT - 1;
			}
		}
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<uint8_t>(BUTT_UP, BUTT_DOWN, g_settings.cyclon.frameTime, 10, 100, 3);
		utils::IrControlValue<float>(BUTT_LEFT, BUTT_RIGHT, g_settings.cyclon.hueIncrease, 0, 8, 1);
		utils::IrControlBool(BUTT_OK, g_settings.cyclon.reflect);
	}
}
