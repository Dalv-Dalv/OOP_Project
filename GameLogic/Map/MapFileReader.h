#ifndef MAPFILEREADER_H
#define MAPFILEREADER_H
#include <string>

#include <vector>

using namespace std;

class MapFileReader {
public:
	static vector<vector<float>> ReadMap(const char* filepath);
};

#endif //MAPFILEREADER_H
