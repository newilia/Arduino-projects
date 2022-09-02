#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "GyverFilters.h"
#include "swordGeometry.h"

GKalman filter(0.1, 0.1);
float charge = 0;
float debugChargeCorrection = 0;

float getSinWave(float x)
{
	float baseHeight = (charge + debugChargeCorrection) * Sword::Height();
	float seconds = millis() * 0.001;
	const float sinAmpl = 1.5;
	const float sinSpeed = 3;
	const float sinScale = 0.4;
	return baseHeight + sinAmpl * sin(seconds * sinSpeed + x * sinScale);
}

void voltageWave()
{
	EVERY_N_MILLIS(20)
	{
		float voltage = utils::GetVoltage();

		charge = max(0, (voltage - LOW_VOLTAGE) / (HIGH_VOLTAGE - LOW_VOLTAGE));
		charge = filter.filtered(charge);

		const int HIGH_VOLTAGE_HUE = 120;
		const int LOW_VOLTAGE_HUE = -15;
		uint8_t hue = LOW_VOLTAGE_HUE + (HIGH_VOLTAGE_HUE - LOW_VOLTAGE_HUE) * charge;
		const CRGB onColor = CHSV(hue, 240, 255);
		const CRGB offColor = CRGB();// = CHSV(HIGH_VOLTAGE_HUE * charge, 255, 30);

		for (int i = 0; i < LEDS_COUNT; ++i)
		{
			auto ledPos = Sword::GetLedPosition(i);
			float sinValue = getSinWave(ledPos.first);
			float mixRatio = sinValue - ledPos.second;
			auto color = utils::MixColors(onColor, offColor, mixRatio);
			g_leds[i] = color;
		}
		
		FastLED.show();
	}
}