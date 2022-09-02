#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "utils.h"

void plainColor()
{
	EVERY_N_MILLIS(20)
	{
		for (auto & led : g_leds)
		{
			led = utils::MixColors(g_settings.plainColor, led, 0.2);
		}
		FastLED.show();
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<uint8_t>(BUTT_DOWN, BUTT_UP, g_settings.plainColor.value, 65, 255, 15);
		utils::IrControlValue<uint8_t>(BUTT_LEFT, BUTT_RIGHT, g_settings.plainColor.hue, 0, 255, 10, true);
		utils::IrControlValue<uint8_t>(BUTT_STAR, BUTT_HASH, g_settings.plainColor.sat, 0, 255, 15);

		if (g_ir.GetKey() == BUTT_OK)
		{
			g_ir.ResetKey();
			for (auto & led : g_leds)
			{
				led = g_settings.plainColor;
			}
		}
	}
}