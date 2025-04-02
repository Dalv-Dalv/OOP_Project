#pragma once
#include <string>

#include <vector>

#include "TerrainData.h"

using namespace std;

class MapFileReader {
public:
	static TerrainData* ReadMap(const char* filepath);
};