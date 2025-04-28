#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "TerrainPoint.h"

using namespace std;

class TerrainData {
private:
	int width, height; // Map width/height
	vector<TerrainPoint> flattenedValues; // Actually a 2D map

public:
	string id;
	void SetValueAt(int x, int y, TerrainPoint value);
	TerrainPoint GetValueAt(int x, int y) const;

	TerrainData(int width, int height);
	TerrainData(int width, int height, vector<TerrainPoint>& flattenedValues);

	TerrainData* ExtractRegion(int startx, int starty, int width, int height) const;

	const int& GetWidth() const;
	const int& GetHeight() const;
	vector<TerrainPoint>& GetFlattenedValues();
	void SetFlattenedValues(vector<TerrainPoint>& flattenedValues);
};