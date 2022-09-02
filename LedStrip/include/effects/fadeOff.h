#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "cylon.h"

void fadeOff()
{
	EVERY_N_MILLIS(10)
	{
		fadeall();
		FastLED.show();
	}
}