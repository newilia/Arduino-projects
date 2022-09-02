#include "utils.h"
#include <Arduino.h>
#include "globalDefs.h"
#include "externVars.h"

void utils::DrawLine(const CRGB & color, float start, float end)
{
    EnsureIncreasing(start, end);

    ConstrFloat(start, 0, LEDS_COUNT);
    ConstrFloat(end, 0, LEDS_COUNT);

    int sf = floor(start);
    int sc = ceil(start);
    int ef = floor(end);

    float startFrac = sf != ef ? sc - start : end - start;

    if (sf >= 0 && sf < LEDS_COUNT)
    {
        g_leds[sf] = MixColors(color, g_leds[sf], startFrac);
    }

    for (int i = sc; i < ef && i < LEDS_COUNT; ++i)
    {
        g_leds[i] = color;
    }

    if (sf != ef && ef >= 0 && ef < LEDS_COUNT)
    {
        float endFrac = end - ef;
        g_leds[ef] = MixColors(color, g_leds[ef], endFrac);
    }
}

float utils::GetPixVisibility(float pos, float zoneStart, float zoneEnd)
{
    float sf = floor(zoneStart);
    float sc = ceil(zoneStart);
    float ef = floor(zoneEnd);

    if (pos == sf)
    {
        float startFrac = sf != ef ? sc - zoneStart : zoneEnd - zoneStart;
        return startFrac;
    }
    if (pos >= sc && pos < ef)
    {
        return 1;
    }
    if (pos == ef && sf != ef)
    {
        float endFrac = zoneEnd - ef;
        return endFrac;
    }
    return 0;
}