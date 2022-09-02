#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "utils.h"
#include "effects/twinkleFox.h"

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING 170

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


const TProgmemRGBPalette16 * palettes[] = {
	&HeatColors_p,
	&RetroC9_p,
	&BlueWhite_p,
	&RainbowColors_p,
	&FairyLight_p,
	&RedGreenWhite_p,
	&PartyColors_p,
	&RedWhite_p,
	&Snow_p,
	&Holly_p,
	&Ice_p
};

const uint8_t palettesCount = sizeof(palettes) / sizeof(TProgmemRGBPalette16*);

CRGB GetColor(uint8_t heat);

void Fire2012WithPalette_impl(bool symmetric, bool reverseDir)
{
	//const CRGBPalette16 gPal = HeatColors_p; //(CRGB::Black, CRGB::Red, CRGB::Orange, CRGB::Yellow);

	// Array of temperature readings at each simulation cell
	static byte heats[LEDS_COUNT];

	// Step 1.  Cool down every cell a little
	for (int i = 0; i < LEDS_COUNT; i++)
	{
		heats[i] = qsub8(heats[i], random8(0, ((COOLING * 10) / LEDS_COUNT) + 2));
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (int k = LEDS_COUNT - 1; k >= 2; k--)
	{
		heats[k] = (heats[k - 1] + heats[k - 2] + heats[k - 2]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if (random8() < SPARKING)
	{
		int y = random8(3);
		heats[y] = qadd8(heats[y], random8(160, 255));
	}

	int ledsCount = symmetric ? round((float)LEDS_COUNT / 2) : LEDS_COUNT;

	// Step 4.  Map from heat cells to LED colors
	for (int j = 0; j < ledsCount; j++)
	{
		// Scale the heat value from 0-255 down to 0-240
		// for best results with color palettes.
		CRGB color = GetColor(heats[j]);// g_settings.fire.palette == 1 ? HeatColor(heat) : PaletteHeatColor(heat);
		/*for (int i = 0; i < 3; ++i)
		{
			color[i] = scale8(color[i], COLORS_SCALE[i]);
		}*/

		int pixelnumber;

		pixelnumber = reverseDir ? (ledsCount - 1) - j : j;
		g_leds[pixelnumber] = color;

		if (symmetric)
		{
			int symmPix = LEDS_COUNT - 1 - pixelnumber;
			g_leds[symmPix] = color;
		}
	}
}

void fire()
{
	EVERY_N_MILLISECONDS_I(fireTimer, g_settings.fire.frameTime)
	{
		fireTimer.setPeriod(g_settings.fire.frameTime);
		// Add entropy to random number generator; we use a lot of it.
		random16_add_entropy(random());

		// Fourth, the most sophisticated: this one sets up a new palette every
		// time through the loop, based on a hue that changes every time.
		// The palette is a gradient from black, to a dark color based on the hue,
		// to a light color based on the hue, to white.
		//
		//   static uint8_t hue = 0;
		//   hue++;
		//   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
		//   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
		//   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

		Fire2012WithPalette_impl(true, false); // run simulation frame, using palette colors
		uint8_t br = FastLED.getBrightness();
		FastLED.setBrightness(g_settings.fire.brightness);
		FastLED.show();							   // display this frame
		FastLED.setBrightness(br);
											   //delay(fireFrameTime);
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<uint8_t>(BUTT_UP, BUTT_DOWN, g_settings.fire.frameTime, 30, 80, 5);
		utils::IrControlValue<uint8_t>(BUTT_LEFT, BUTT_RIGHT, g_settings.fire.brightness, 20, 200, 20);
		utils::IrControlValue<uint8_t>(BUTT_OK, g_settings.fire.paletteIndex, 0, palettesCount, 1);
	}
}

uint8_t norm(uint8_t val)
{
	float fVal = ((float)val / 255 - 0.5) *  M_PI;
	return (0.5 + 0.5 * sin(fVal)) * 255;
}

CRGB GetColor(uint8_t heat)
{
	auto palette = palettes[g_settings.fire.paletteIndex];
	bool isHeatColors = palette == &HeatColors_p;
	
	if (isHeatColors)
	{
		heat = scale8(heat, 220);
	}
	auto color = ColorFromPalette(*palette, heat);

	if (isHeatColors)
	{
		const CRGB heatColorCorrection(150, 130, 120);
		color.nscale8(heatColorCorrection);
	}
	else
	{
		color.nscale8(norm(heat));
	}
	return color;
}