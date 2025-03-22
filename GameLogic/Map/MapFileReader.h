#ifndef MAPFILEREADER_H
#define MAPFILEREADER_H
#include <string>

#include <vector>

#include "TerrainData.h"

using namespace std;

class MapFileReader {
public:
	static TerrainData* ReadMap(const char* filepath);
};

#endif //MAPFILEREADER_H
