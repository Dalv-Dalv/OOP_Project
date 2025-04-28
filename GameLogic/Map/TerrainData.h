#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "TerrainPoint.h"

using namespace std;

class TerrainData {
private:
	int width, height; // Map width/height
	vector<TerrainPoint> terrainValues; // Actually a 2D map
	vector<FTerrainPoint> fTerrainValues;

public:
	string id;
	void SetValueAt(int x, int y, TerrainPoint value);
	void SetFValueAt(int x, int y, FTerrainPoint value);
	TerrainPoint GetValueAt(int x, int y) const;
	FTerrainPoint GetFValueAt(int x, int y) const;

	TerrainData(int width, int height);
	TerrainData(int width, int height, vector<TerrainPoint>& terrainValues);

	TerrainData* ExtractRegion(int startx, int starty, int width, int height) const;

	const int& GetWidth() const;
	const int& GetHeight() const;
	vector<TerrainPoint>& GetTerrainValues();
	void SetTerrainValues(const vector<TerrainPoint>& terrainValues);
};