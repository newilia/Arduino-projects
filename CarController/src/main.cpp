#include <Arduino.h>
#include <Servo.h>

#include "GyverEncoder.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "chars.h"

const int THERM1_PIN = A1;
const int THERM2_PIN = A2;

const int SERVO_TEMPERATURE_PIN = 3;
const int SERVO_UP_PIN = 5;
const int SERVO_DOWN_PIN = 6;

const int ANALOG_IN_PIN1 = A5; //free
const int ANALOG_IN_PIN2 = A6; //free

const int ENC1_KEY_PIN = A3;
const int ENC1_S1_PIN = 4;
const int ENC1_S2_PIN = 2;

const int ENC2_KEY_PIN = 7;
const int ENC2_S1_PIN = 9;
const int ENC2_S2_PIN = 8;

const int LCD_PIN_DATA = A4;
const int LCD_PIN_CLOCK = A5;

const int FAN_PIN = 11;

Encoder enc1(ENC1_S1_PIN, ENC1_S2_PIN, ENC1_KEY_PIN, true);
Encoder enc2(ENC2_S1_PIN, ENC2_S2_PIN, ENC2_KEY_PIN, true);

Servo servoTemp;
Servo servoUp;
Servo servoDown;
const float servoDegreesPerSec = 90;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const uint8_t PARAMS_VERSION = 1;
struct Params
{
	uint8_t dummy;
	uint8_t version = PARAMS_VERSION;
	float flowSelectorValue = 0;
	uint8_t tempSelectorValue = 0;
} currentParams;

const float flowSelectorLimit = 4;
const float flowSelectorStep = 0.25;

const float tempSelectorStepCount = 16;
int tempServoAngle = 0;

void setup() 
{
	Serial.begin(115200);
	lcd.init();
	lcd.backlight();
	initChars();
	analogReference(EXTERNAL);
	pinMode(THERM1_PIN, INPUT);
	//pinMode(FAN_PIN, OUTPUT);

	//разгон шим
	// Пины D3 и D11 - 31.4 кГц
	//TCCR2B = 0b00000001;  // x1
	//TCCR2A = 0b00000001;  // phase correct
	// Пины D3 и D11 - 8 кГц
	//TCCR2B = 0b00000010;  // x8
	//TCCR2A = 0b00000011;  // fast pwm

	//pinMode(ANALOG_IN_PIN1, INPUT_PULLUP);
	//pinMode(ANALOG_IN_PIN2, INPUT_PULLUP);

	{	// load settings
		eeprom_read_block(&currentParams, 0, sizeof(currentParams));
		if (currentParams.version == PARAMS_VERSION)
		{
			Serial.print("Settings loaded, version = ");
			Serial.println(currentParams.version);
		}
		else
		{
			Serial.print("Current settings version = ");
			Serial.print(PARAMS_VERSION);
			Serial.print(", but stored = ");
			Serial.println(currentParams.version);
			currentParams = Params();
		}
	}
	
	servoTemp.attach(SERVO_TEMPERATURE_PIN);
	servoUp.attach(SERVO_UP_PIN);
	servoDown.attach(SERVO_DOWN_PIN);
}

bool initted = false;
uint32_t temperatureReadMs = 0;
uint32_t paramsChangeMs = 0;

struct 
{
	struct 
	{
		int target = 0;
		float current = 0;
	} down, up;
} servoAngles;


float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void mainScreen()
{
	// temp
	if (enc1.isLeft())
	{
		currentParams.tempSelectorValue = constrain(currentParams.tempSelectorValue - 1, 0, tempSelectorStepCount);
	}
	else if (enc1.isRight())
	{
		currentParams.tempSelectorValue = constrain(currentParams.tempSelectorValue + 1, 0, tempSelectorStepCount);
	}

	// flows
	if (enc2.isLeft())
	{
		currentParams.flowSelectorValue -= flowSelectorStep;
		if (currentParams.flowSelectorValue < 0) currentParams.flowSelectorValue += flowSelectorLimit;
	}
	else if (enc2.isRight())
	{
		currentParams.flowSelectorValue += flowSelectorStep;
		if (currentParams.flowSelectorValue >= flowSelectorLimit) currentParams.flowSelectorValue -= flowSelectorLimit;
	}

	// apply input
	if (enc1.isTurn() || enc2.isTurn() || !initted)
	{
		if (initted)		paramsChangeMs = millis();

		const uint8_t coldDeadZone = 3;
		const uint8_t hotDeadZone = 12;
		float mappedTempValue;
		if (currentParams.tempSelectorValue == 0) 
		{
			mappedTempValue = 0;
		} 
		else if (currentParams.tempSelectorValue == tempSelectorStepCount)
		{
			mappedTempValue = tempSelectorStepCount;
		} 
		else
		{
			mappedTempValue = constrain(mapf(currentParams.tempSelectorValue, 1, tempSelectorStepCount - 1, coldDeadZone, hotDeadZone) / tempSelectorStepCount, 0, 1);
		}
		tempServoAngle = map(180 * mappedTempValue, 0, 180, 180, 0);
	
		auto & x = currentParams.flowSelectorValue;
		float downServoValue = constrain(1.5 - abs(x - 2.5), 0, 1);
		servoAngles.down.target = map(downServoValue * 180, 0, 180, 180, 0);

		float upServoValue = constrain(1.5 - abs(x - 1.5), 0, 1);
		servoAngles.up.target = map(upServoValue * 180, 0, 180, 180, 16);

		// therm
		lcd.setCursor(1, 1);
		lcd.write(THERMOMETER);
		lcd.setCursor(0, 0);
		float tempNormValue = (float)currentParams.tempSelectorValue / tempSelectorStepCount;
		printBar((tempNormValue - 0.5) * 2);
		lcd.setCursor(0, 1);
		printBar(tempNormValue * 2);

		//flows
		lcd.setCursor(14, 0);
		lcd.write('^');
		lcd.setCursor(15, 0);
		printBar(upServoValue);

		lcd.setCursor(14, 1);
		lcd.write('v');
		lcd.setCursor(15, 1);
		printBar(downServoValue);

	}

	if (paramsChangeMs != 0 && (millis() - paramsChangeMs > 2000))
	{
		paramsChangeMs = 0;
		eeprom_update_block(&currentParams, 0, sizeof(currentParams));
		Serial.println("settings saved");
	}

	static uint32_t temperatureMs = 0;
	if ((millis() - temperatureMs) > 300 || !initted)
	{
		temperatureMs = millis();
		float aRef = 3.3;
		float thermistorVoltage = aRef * analogRead(THERM1_PIN) / 1024;
		float resistorOhms = 9750;
		float thermistorResistance = (aRef - thermistorVoltage) * resistorOhms / thermistorVoltage;

		float Kelvin = 273.15;
		float T1 = 35 + Kelvin;
		float R1 = 6300;
		float T2 = 100 + Kelvin;
		float R2 = 670;
		float B = 3950;//3950;

		float T = (B * T1) / (B + (T1 * log(thermistorResistance / R1))) - Kelvin;
		//float T_ = 1.f / ((1.f / T1) + (1.f / B) * log(thermistorResistance / R1)) - Kelvin;

		lcd.setCursor(8, 0);
		lcd.print("    ");
		lcd.setCursor(3, 0);
		char tempStr[8];
		sprintf(tempStr, "T in =%3d%c", (int)T, B11011111);
		lcd.print(tempStr);

		/*lcd.setCursor(7, 1);
		lcd.print("     ");
		lcd.setCursor(3, 1);
		sprintf(tempStr, "R = %d", (int)thermistorResistance);
		lcd.print(tempStr);*/

		//todo print temp if changed only
	}

	if (!initted)
	{
		initted = true;
		servoAngles.down.current = servoAngles.down.target;
		servoAngles.up.current = servoAngles.up.target;
	}

	static uint32_t servoUpdateMs;
	int dt = millis() - servoUpdateMs;
	const uint8_t targetDt = 10;
	if (dt > targetDt)
	{
		if (servoAngles.down.current < servoAngles.down.target)
		{
			servoAngles.down.current = min(servoAngles.down.current + servoDegreesPerSec * dt / 1000, servoAngles.down.target);
		}
		else if (servoAngles.down.current > servoAngles.down.target)
		{
			servoAngles.down.current = max(servoAngles.down.current - servoDegreesPerSec * dt / 1000, servoAngles.down.target);
		}
		
		if (servoAngles.up.current < servoAngles.up.target)
		{
			servoAngles.up.current = min(servoAngles.up.current + servoDegreesPerSec * dt / 1000, servoAngles.up.target);
		}
		else if (servoAngles.up.current > servoAngles.up.target)
		{
			servoAngles.up.current = max(servoAngles.up.current - servoDegreesPerSec * dt / 1000, servoAngles.up.target);
		}
		
		servoUpdateMs = millis();
	}
	
	
	servoDown.write(servoAngles.down.current);
	servoUp.write(servoAngles.up.current);
	servoTemp.write(tempServoAngle);
}

void loop() 
{
	enc1.tick();
	enc2.tick();
	mainScreen();
}

