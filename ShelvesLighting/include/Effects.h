#pragma once
#include <FastLED.h>
#include <ExternVars.h>

namespace Effects
{
	class IEffect
	{
	public:
		virtual void Show() = 0;
	};

	class FadingColor : public IEffect
	{
	public:
		void Show() override
		{
			EVERY_N_MILLIS_I(period, 10)
			{
				int brightness = (millis() / 8) % 512;
				if (brightness > 255)
				{
					brightness = 511 - brightness;
				}
				FastLED.showColor(CHSV(40, 255, brightness));
			}
		}
	};
}
