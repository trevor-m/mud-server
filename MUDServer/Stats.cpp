#include "Stats.h"

Stats::Stats(void)
{
	for(int i = 0; i < STAT_COUNT; i++)
	{
		this->stats[i] = 19;
	}
}

int Stats::GetStat(Stat stat)
{
	return stats[stat];
}