#pragma once
#include "externVars.h"
// effects
#include "effects/fire.h"
#include "effects/cylon.h"
#include "effects/twinkleFox.h"
#include "effects/pacifica.h"
#include "effects/fadeOff.h"
#include "effects/plainColor.h"
#include "effects/voltageWave.h"
#include "effects/roundPulse.h"
#include "effects/runningLine.h"
#include "effects/radialGradient.h"
#include "effects/christmas.h"

typedef void (*LedEffect)();
LedEffect g_effects[] = {christmas, voltageWave, plainColor, runningLinesAnimation, cylon, fire, 
	twinkleFox, pacifica, roundPulse, radialGradient};

const uint8_t effectsCount = sizeof(g_effects) / sizeof(LedEffect);

void animationMain()
{
	if (g_settings.animationMode >= 0 && g_settings.animationMode < effectsCount)
	{
		g_effects[g_settings.animationMode]();
	}
	else
	{
		fadeOff();
	}
}