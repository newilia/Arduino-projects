#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "utils.h"

typedef void (*ChristmasMode)();
void mode1();
void mode2();
void mode3();
void twinkle();

namespace
{
	auto & s = g_settings.christmas;
	const int chCount = 4;
	
	struct
	{
		float 	intensivity = 1;
		CRGB 	color;
	} channels [chCount];

	ChristmasMode modes[] = {twinkle, mode1, mode2, mode3};
	const int modesCount = sizeof(modes) / sizeof(ChristmasMode);
	const float fadeSpeed = 0.17;
	bool modeChanged = true;
	
	const bool colorCombinations[4][4] = 
	{
		{0, 1, 0, 1},
		{0, 1, 1, 0},
		{1, 0, 1, 0},
		{1, 0, 0, 1}
	};
};

void updateColors()
{
	channels[0].color = CHSV(80, 	220 * s.sat, 	220 * s.brightness); //green
	channels[1].color = CHSV(27, 	255 * s.sat, 	255 * s.brightness); //yellow
	channels[2].color = CHSV(5, 	220 * s.sat, 	210 * s.brightness); //red
	channels[3].color = CHSV(145, 	185 * s.sat, 	210 * s.brightness); //blue
}

void lightChannel(uint8_t ch, bool enabled)
{
	auto & in = channels[ch].intensivity;
	if (enabled)
	{
		in += (1 - in) * fadeSpeed; 
	}
	else
	{
		in -= in * fadeSpeed * 4;
	}
}

void christmas()
{
	static bool initialized;
	if (!initialized)
	{
		updateColors();
		initialized = true;
	}

	EVERY_N_MILLIS(20)
	{
		if (g_ir.KeyPressed())
		{
			updateColors();
		}
		if (g_ir.GetKey() == BUTT_OK)
		{
			modeChanged = true;
		}
		modes[s.mode]();
		modeChanged = false;

		const uint8_t interval = 4;
		for (int i = 0; i < LEDS_COUNT; ++i)
		{
			if (i % interval)
			{
				g_leds[i] = CRGB();
				continue;
			}
			uint8_t ch = (i / interval) % chCount;
			g_leds[i] = utils::MixColors(channels[ch].color, CRGB(), channels[ch].intensivity); 
		}

		
		uint8_t prevBr = FastLED.getBrightness();
		FastLED.setBrightness(255);
		FastLED.show();
		FastLED.setBrightness(prevBr);
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<float>(BUTT_DOWN, BUTT_UP, s.brightness, 0.2, 1, 0.1);
		utils::IrControlValue<float>(BUTT_STAR, BUTT_HASH, s.sat, 0.2, 1, 0.1);
		utils::IrControlValue<uint8_t>(BUTT_OK, s.mode, 0, modesCount - 1, 1);
	}
}

void mode1()
{
	float brighness = 0.5 * (1 + utils::PhaseModulation(7, 1, 3.5));
	for (int i = 0; i < chCount; ++i)
	{
		channels[i].intensivity = brighness;
	}
}

void mode2()
{
	static uint8_t firstCh = 0;
	static uint8_t delayIndex = 0;

	const int delays[3] = {600, 300, 150};

	EVERY_N_MILLISECONDS_I(changeColorsTimer, delays[0])
	{
		++firstCh;
		firstCh %= chCount;
	}

	EVERY_N_SECONDS(10)
	{
		++delayIndex;
		delayIndex %= 3;
		changeColorsTimer.setPeriod(delays[delayIndex]);
	}
	
	for (int ch = 0; ch < chCount; ++ch)
	{
		lightChannel(ch, ch == firstCh || ch == ((firstCh+1) % chCount));
	}
}

void mode3()
{
	for (int i = 0; i < chCount; ++i)
	{
		channels[i].intensivity = max(0, utils::bpmSin(5, M_PI * 2 / chCount * i));
	}
}

void twinkle()
{
	static bool mode = false;
	const int period = 5200;

	EVERY_N_MILLIS(period / 2)
	{
		mode = !mode;
	}

	static uint8_t phase;
	static bool chState[chCount];
	
	EVERY_N_MILLIS(period / 64)
	{
		++phase;
		phase %= 64;
		bool oddPhase = phase % 2;

		for (int ch = 0; ch < chCount; ++ch)
		{
			if (mode)
			{
				uint8_t firstCh = phase / 2 % chCount;
				uint8_t secondCh = (firstCh + 1) % chCount;
				chState[ch] = ch == firstCh || ch == secondCh;
			}
			else
			{
				uint8_t firstCh = phase / 4 % chCount;
				uint8_t secondCh = (firstCh + 1) % chCount;
				chState[ch] = (ch == firstCh && oddPhase) || (ch == secondCh && !oddPhase);
			}
		}
	}

	for (int ch = 0; ch < chCount; ++ch)
	{
		lightChannel(ch, chState[ch]);
	}
}