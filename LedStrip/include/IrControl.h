#pragma once
#include <Arduino.h>
#include <IRLremote.h>
#include "irCodes.h"

class IrControl
{
public:
	bool Setup();
	bool Receive();

	bool KeyPressed() { return m_key != 0; }
	uint32_t GetKey() { return m_key; }
	int8_t GetDigit() { return getKeyDigit(m_key); }
	void ResetKey() { m_key = 0; }
	time_t GetLastReceiveTime() { return m_lastReceiveTime; }
	//void SetOnReceive(void (*func)(uint32_t)) { m_onReceive = func; }

private:
	CHashIR 	m_receiver;
	uint32_t 	m_key = 0;
	time_t		m_lastReceiveTime = 0;
	//void 		(*m_onReceive)(uint32_t);
};