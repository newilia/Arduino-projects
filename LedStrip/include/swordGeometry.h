#pragma once
#include "utils.h"

namespace Sword
{
	constexpr int Width() { return 8; }
	constexpr int Height() { return 17; }

	Pair<float, float> GetLedPosition(int num)
	{
		Pair<float, float> pos;

		// if (num <= 5)
		// {
		// 	pos.second = num;
		// }
		// else if ((num >= 6 && num <= 8) || (num >= 37 && num <= 39))
		// {
		// 	pos.second = 6;
		// }
		// else if (num == 9 || num == 36)
		// {
		// 	pos.second = 7;
		// }
		// else if ((num >= 10 && num <= 12) || (num >= 33 && num <= 35))
		// {
		// 	pos.second = 8;
		// }
		// else if (num >= 13 && num <= 22)
		// {
		// 	pos.second = num - 4;
		// }
		// else if (num >= 23 && num <= 32)
		// {
		// 	pos.second = 41 - num;
		// }
		// else if (num >= 40 && num <= 44)
		// {
		// 	pos.second = 45 - num;
		// }


		// if (num == 9)
		// {
		// 	pos.first = 0;
		// }
		// else if (num >= 6 && num <= 8)
		// {
		// 	pos.first = 9 - num;
		// }
		// else if (num >= 10 && num <= 12)
		// {
		// 	pos.first = num - 9;
		// }
		// else if ((num >= 1 && num <= 5) || (num >= 13 && num <= 22))
		// {
		// 	pos.first = 4;
		// }
		// else if (num == 0)
		// {
		// 	pos.first = 5;
		// }
		// else if ((num >= 23 && num <= 32) || (num >= 40 && num <= 44))
		// {
		// 	pos.first = 6;
		// }
		// else if (num >= 33 && num <= 35)
		// {
		// 	pos.first = num - 26;
		// }
		// else if (num == 36)
		// {
		// 	pos.first = 10;
		// }
		// else if (num >= 37 && num <= 39)
		// {
		// 	pos.first = 46 - num;
		// }
		if (num == 0)
		{
			pos.second = 0.5;
		}
		if (num >= 1 && num <= 6)
		{
			pos.second = num;
		}
		else if ((num >= 6 && num <= 9) || (num >= 40 && num <= 43))
		{
			pos.second = 6;
		}
		else if (num == 10 || num == 39)
		{
			pos.second = 6.5;
		}
		else if ((num >= 11 && num <= 14) || (num >= 35 && num <= 38))
		{
			pos.second = 7;
		}
		else if (num >= 14 && num <= 23)
		{
			pos.second = num - 7;
		}
		else if (num == 24 || num == 25)
		{
			pos.second = 16.7;
		}
		else if (num >= 26 && num <= 35)
		{
			pos.second = 42 - num;
		}
		else if (num >= 43 && num <= 48)
		{
			pos.second = 49 - num;
		}


		if (num == 10)
		{
			pos.first = 0.5;
		}
		else if (num >= 6 && num <= 9)
		{
			pos.first = 10 - num;
		}
		else if (num >= 11 && num <= 14)
		{
			pos.first = num - 10;
		}
		else if ((num >= 1 && num <= 6) || (num >= 14 && num <= 23))
		{
			pos.first = 4;
		}
		else if (num == 0)
		{
			pos.first = 4.5;
		}
		else if (num == 24)
		{
			pos.first = 4.33;
		}
		else if (num == 25)
		{
			pos.first = 4.66;
		}
		else if ((num >= 26 && num <= 35) || (num >= 43 && num <= 48))
		{
			pos.first = 5;
		}
		else if (num >= 35 && num <= 38)
		{
			pos.first = num - 30;
		}
		else if (num == 39)
		{
			pos.first = 8.5;
		}
		else if (num >= 40 && num <= 43)
		{
			pos.first = 48 - num;
		}

		return pos;
	}
}