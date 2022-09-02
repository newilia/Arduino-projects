#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "globalDefs.h"
#include "externVars.h"

const int FRAME_TIME = 20;
void runningLinesAnimation();

class RunningLine
{
public:
	enum Form
	{
		EXP = 0,
		SQUARE, //bug in this mode
		SIN,
		_COUNT
	};

	RunningLine() = default;

	RunningLine(CRGB color, float pos, float speed, float size, Form form, float opacity)
	: m_position(pos)
	, m_size(size)
	, m_color(color)
	, m_form(form)
	, m_opacity(opacity)
	, m_shiftPerTick(speed)
	{
	}

	void Update();
	void SetColor(CRGB color) { m_color = color; }
	void SetForm(Form form) { m_form = form; }
	void SetSpeed(float speed) { m_shiftPerTick = speed; }
	void SetSize(uint8_t size) { m_size = size; }
	void SetOpecity(float opacity) { m_opacity = opacity; }

protected:
	void Draw();
	float GetPosBrighness(int32_t pos);
	virtual float GetSpeed() { return m_shiftPerTick; }

	float m_position = 0;
	float m_shiftPerTick = 0;
	float m_size = 0;
	CRGB m_color;
	Form m_form = Form::SQUARE;
	float m_opacity = 1;
};