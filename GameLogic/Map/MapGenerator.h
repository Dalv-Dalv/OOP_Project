#pragma once
#include <vector>

#include <raylib.h>

using namespace std;

class MapGenerator {
private:
	static vector<char> caseIndexToEdges[];
	static pair<Vector2, Vector2> CalculateEdge(const float surfaceLevel, const float interpolationLevel, const char lineIndex);
	static vector<pair<Vector2, Vector2>> GenerateLineList(float surfaceLevel, float interpolationAmount, float mapScale, const vector<vector<float>>& map);
public:
	static void GenerateMap(const float surfaceLevel, const float interpolationAmount, const vector<vector<float>>& map);

};
