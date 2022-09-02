#include "IrControl.h"
#include "globalDefs.h"
#include "irCodes.h"
#include <IRLremote.h>
#include <Arduino.h>

const uint8_t KEY_TIMEOUT = 150;

bool IrControl::Setup()
{
	return m_receiver.begin(IR_PIN);
}

bool IrControl::Receive()
{
	time_t ms = millis();
	if (m_receiver.available())
	{
		m_lastReceiveTime = ms;
		auto cmd = m_receiver.read().command;
		if (cmd != KEY_REPEAT)
		{
			m_key = cmd;
		}
	}
	else
	{
		if (m_key != 0 && ms - m_lastReceiveTime > KEY_TIMEOUT)
		{
			m_key = 0;
		}
	}
	return m_receiver.receiving();
}