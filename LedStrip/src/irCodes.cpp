#include "irCodes.h"

int getKeyDigit(HashIR_command_t code)
{
#if REMOTE_TYPE > 0
	if (code == BUTT_0)
	{
		return 0;
	}
	if (code == BUTT_1)
	{
		return 1;
	}
	if (code == BUTT_2)
	{
		return 2;
	}
	if (code == BUTT_3)
	{
		return 3;
	}
	if (code == BUTT_4)
	{
		return 4;
	}
	if (code == BUTT_5)
	{
		return 5;
	}
	if (code == BUTT_6)
	{
		return 6;
	}
	if (code == BUTT_7)
	{
		return 7;
	}
	if (code == BUTT_8)
	{
		return 8;
	}
	if (code == BUTT_9)
	{
		return 9;
	}
#endif
	return -1;
}
