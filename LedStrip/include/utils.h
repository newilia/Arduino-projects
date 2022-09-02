#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"

#define LOG_VALUE(x) Serial.print(#x); Serial.print(" = "); Serial.println(x);
#define LOG_VALUES(x,y) Serial.print(#x); Serial.print("="); Serial.print(x); Serial.print("; "); Serial.print(#y); Serial.print("="); Serial.println(y);


template<typename M, typename N>
struct Pair
{
	M first;
	N second;
};

namespace utils
{
	void DrawLine(const CRGB & color, float start, float end);

    float GetPixVisibility(float pos, float start, float end);

    inline int Mod(int32_t value, int32_t N)
    {
        if (value < 0)
        {
            return (N - (-value % N)) % N;
        }
        else
        {
            return value % N;
        }
    }

    template<typename T>
    inline void Swap(T & val1, T & val2)
    {
        T temp = val1;
        val1 = val2;
        val2 = temp;
    }

    template<typename T>
    void EnsureIncreasing(T & val1, T & val2)
    {
        if (val1 > val2)
        {
            Swap(val1, val2);
        }
    }

	inline void ConstrFloat(float & val, float start, float end)
    {
        EnsureIncreasing(start, end);
        if (val < start)
        {
            val = start;
        }
        else if (val > end)
        {
            val = end;
        }
    }

	inline CRGB MixColors(const CRGB & led1, const CRGB & led2, float ratio)
    {
        ConstrFloat(ratio, 0, 1);
        CRGB result;
        for (int i = 0; i < 3; ++i)
        {
            result[i] = led1[i] * ratio + led2[i] * (1 - ratio);
        }
        return result;
    }

    inline void FilterRGB(CRGB * target, const CRGB & filter)
    {
        for (int i = 0; i < 3; ++i)
        {
            (*target)[i] *= (float)filter[i] / 255;
        }
    }

	inline float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    template<typename T>
    void IrControlValue(uint32_t decrKey, uint32_t incrKey, T &val, T low, T high, T step, bool round = false)
    {
        if (g_ir.GetKey() == decrKey)
        {
            T newVal = val - step;
            if (newVal < val && newVal >= low)
            {
                val = newVal;
            }
            else
            {
                if (round)
                {
                    val = high;
                }
                else
                {
                    val = low;
                    FastLED.showColor(CRGB());
                }
            }
        }
        else if (g_ir.GetKey() == incrKey)
        {
            T newVal = val + step;
            if (newVal > val && newVal <= high)
            {
                val = newVal;
            }
            else
            {
                if (round)
                {
                    val = low;
                }
                else
                {
                    val = high;
                    FastLED.showColor(CRGB());
                }
            }
        }
    }

    template<typename T>
    void IrControlValue(uint32_t incrKey, T &val, T low, T high, T step)
    {
        if (g_ir.GetKey() == incrKey)
        {
            g_ir.ResetKey();
            T newVal = val + step;
            if (newVal > val && newVal <= high)
            {
                val = newVal;
            }
            else
            {
                val = low;
            }
        }
    }

    inline void IrControlBool(uint32_t switchKey, bool &val)
    {
        if (g_ir.GetKey() == switchKey)
        {
            val = !val;
            g_ir.ResetKey();
        }
    }

    inline float GetVoltage()
    {
        return analogRead(VOLTAGE_PIN) / 1024.0 * 5;
    }

    inline float bpmSin(float bpm, float phaseShift = 0)
    {
        return sin(phaseShift + float(millis()) / 1000 / 60 * 6.28 * bpm);
    }

    inline float PhaseModulation(float mainFreqBpm, float modulatingFreqBpm, float modulationScale)
    {
        float phase = float(millis()) / 1000 / 60 * 6.28 * mainFreqBpm + modulationScale * bpmSin(modulatingFreqBpm);
        return sin(phase);
    }
}