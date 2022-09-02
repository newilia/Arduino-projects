#include "globalDefs.h"
#include <FastLED.h>
//#include "GyverEncoder.h"
#include "irCodes.h"
#include "animationMain.h"
#include "settings.h"
#include "IrControl.h"

CRGB g_leds[LEDS_COUNT];
//Encoder g_enc1(ENC1_S2_PIN, ENC1_S1_PIN, ENC1_KEY_PIN, true);
Settings g_settings;
IrControl g_ir;
time_t g_currentMs = 0;
time_t g_lastFrameMs = 0;
bool g_settingsChanged = false;

void setup()
{
	pinMode(VOLTAGE_PIN, INPUT);
	Serial.begin(115200);
	FastLED.addLeds<WS2812, LEDS_PIN, GRB>(g_leds, LEDS_COUNT);
	FastLED.setBrightness(127);

	if (!g_ir.Setup())
	{
		Serial.print("CIRL_Receive.begin == false! Check IR pin, interrupts required.");
	}

	{	// load settings
		eeprom_read_block(&g_settings, 0, sizeof(g_settings));
		if (g_settings.version == SETTINGS_VERSION)
		{
			Serial.print("Settings loaded, version = ");
			Serial.println(g_settings.version);
		}
		else
		{
			Serial.print("Current settings version = ");
			Serial.print(SETTINGS_VERSION);
			Serial.print(", but stored = ");
			Serial.println(g_settings.version);
			g_settings = Settings();
		}
	}
}

/*void encTick()
{
	g_enc1.tick();

	if (g_enc1.isLeft())
	{
		--g_mode;
		if (g_mode < 0)
		{
			g_mode = getEffectsCount() - 1;
		}
	}
	if (g_enc1.isRight())
	{
		++g_mode;
		if (g_mode == getEffectsCount())
		{
			g_mode = 0;
		}
	}

}*/
bool isDischarged = false;
void loop()
{
	g_currentMs = millis();

	if (g_ir.Receive())
	{
		return;
	}

	if (g_ir.KeyPressed() && !g_settingsChanged)
	{
		g_settingsChanged = true;
	}
	
	const int saveDelay = 1000;
	if (g_currentMs - g_ir.GetLastReceiveTime() > saveDelay && g_settingsChanged)
	{
		g_settingsChanged = false;
		eeprom_update_block(&g_settings, 0, sizeof(g_settings));
		Serial.println("settings saved");
	}

	if (g_ir.GetDigit() > -1)
	{
		g_settings.animationMode = g_ir.GetDigit();
	}
	
	EVERY_N_SECONDS(1)
	{
		float voltage = utils::GetVoltage();
		LOG_VALUE(voltage);
		if (voltage < DISCHARGE_VOLTAGE)
		{
			isDischarged = true;
		}
	}

	if (isDischarged)
	{
		EVERY_N_MILLIS(10)
		{
			FastLED.showColor(millis() % 2000 < 50 ? CRGB(40, 0, 0) : CRGB());
		}
	}
	else
	{
		animationMain();
	}
	
}
