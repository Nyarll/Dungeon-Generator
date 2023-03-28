#include <iostream>
#include "MapGenerator.h"

int main()
{
	MapGenerator generator = MapGenerator();
	generator.Generate(64, 64, 10);
	generator.ShowMap();
	return 0;
}