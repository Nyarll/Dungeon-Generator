#include "Random.h"
#include <random>

Random::Random()
{
}

int Random::Range(int min, int max)
{
	int ans = -1;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> r(min, max);
	ans = r(mt);
	return ans;
}

bool Random::Jadge(int rate)
{
	return (Range(0, 100) <= rate);
}
