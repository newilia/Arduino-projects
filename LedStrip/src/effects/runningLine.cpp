#include "effects/runningLine.h"
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"
#include "utils.h"

const float opacity = 1;
auto & s = g_settings.runningLine;
RunningLine line; //(CRGB(), 0, s.speed, s.size, (RunningLine::Form)s.form, opacity);

void fillBackground(CRGB backgroundColor)
{
	for (auto & led : g_leds)
	{
		led = backgroundColor;
	}
}

void runningLinesAnimation()
{
	EVERY_N_MILLIS(FRAME_TIME) 
	{
		fillBackground(CRGB());

		s.hue += 0.2;
		if (s.hue >= 255)
		{
			s.hue = 0;
		}

		line.SetColor(CHSV(s.hue + 100, 255, s.brightness));
		line.SetForm((RunningLine::Form)g_settings.runningLine.form);
		line.SetSpeed(g_settings.runningLine.speed);
		line.SetSize(g_settings.runningLine.size);
		line.Update();
		
		FastLED.show();
	}

	EVERY_N_MILLIS(100)
	{
		utils::IrControlValue<float>(BUTT_DOWN, BUTT_UP, g_settings.runningLine.speed, -0.5, 0.5, 0.05);
		utils::IrControlValue<uint8_t>(BUTT_STAR, BUTT_HASH, g_settings.runningLine.size, 2, LEDS_COUNT, 2);
		utils::IrControlValue<uint8_t>(BUTT_LEFT, BUTT_RIGHT, g_settings.runningLine.brightness, 50, 255, 15);
		utils::IrControlValue<uint8_t>(BUTT_OK, s.form, 0, (uint8_t)RunningLine::_COUNT - 1, 1);
	}
}

/*********************************/

void RunningLine::Update()
{
	m_position += GetSpeed();
	Draw();		
}

float RunningLine::GetPosBrighness(int32_t pos)
{
	switch (m_form)
	{
	case Form::SQUARE:
		return 1.f - 0.1 * sin((pos - m_position) * M_PI / m_size);

	case Form::SIN:
		return sin((pos - m_position) * M_PI / m_size);

	case Form::EXP:
		const float powBase = 4;
		const float sizeConst = 6;
		if (m_shiftPerTick < 0)
			return min(1, pow(powBase, (m_position - pos) / m_size * sizeConst));
		else
			return min(1, pow(powBase, (-m_position + pos - m_size) / m_size * sizeConst));
	
	default:
		return 1.f;
	}
}

void RunningLine::Draw()
{
	const int32_t start = floor(m_position);
	const int32_t end = ceil(m_position + m_size);

	for (int32_t pos = start; pos < end; ++pos)
	{
		float posBrightness = GetPosBrighness(pos);
		float visibilityValue = utils::GetPixVisibility(pos, m_position, m_position + m_size);
		
		int led = utils::Mod(pos, LEDS_COUNT);
		g_leds[led] = utils::MixColors(m_color, g_leds[led], posBrightness * visibilityValue * m_opacity);
	}
}