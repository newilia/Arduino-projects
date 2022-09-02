#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"

const uint8_t SETTINGS_VERSION = 7;
struct Settings
{
	uint8_t		dummyByte;
	uint8_t 	version = SETTINGS_VERSION;
	uint8_t		animationMode = 1;

	CHSV plainColor {0, 0, 127};

	struct
	{
		uint8_t		frameTime = 50;
		uint8_t		brightness = 150;
		uint8_t		paletteIndex = 0;
	} fire;

	struct 
	{
		float speed = 1.f;
	} pacifica;

	struct 
	{
		bool 	isGlow = true;
		uint8_t frameTime = 20;
		uint8_t	hueStep = 115;
	} roundPulse;
	
	struct 
	{
		float speed = 0.1;
		uint8_t size = 30;
		uint8_t form = 0;
		float hue = 0;
		uint8_t brightness = 200;
	} runningLine;

	struct 
	{
		uint8_t speed = 4;
		uint8_t destiny = 5;
		bool 	changePalette = true;
	} twinkleFox;	
	
	struct
	{
		uint8_t frameTime = 25;
		float	hueIncrease = 1;
		bool	reflect = true;
	} cyclon;

	struct 
	{
		float 	speed = 0.05;
		float 	scale = 10;
		bool	dirOut = true;
		uint8_t brightness = 127;
	} radialGradient;

	struct 
	{
		float brightness = 1;
		float sat = 1;
		uint8_t mode = 0;
	} christmas;
};