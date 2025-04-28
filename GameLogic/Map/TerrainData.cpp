#include "TerrainData.h"

#include <iostream>

#include <raylib.h>

void TerrainData::SetValueAt(int x, int y, TerrainPoint value) {
	if(x < 0 || x >= width) return;
	if(y < 0 || y >= height) return;

	terrainValues[y * width + x] = value;
}
void TerrainData::SetFValueAt(int x, int y, FTerrainPoint value) {
	if(x < 0 || x >= width) return;
	if(y < 0 || y >= height) return;

	terrainValues[y * width + x].surfaceValue = (unsigned char)(value.surfaceValue * 255);
	terrainValues[y * width + x].oreValue = (unsigned char)(value.oreValue * 255);
	fTerrainValues[y * width + x] = value;
}


TerrainPoint TerrainData::GetValueAt(int x, int y) const {
	if(x < 0 || x >= width) return {0, 0, 0};
	if(y < 0 || y >= height) return {0, 0, 0};

	return terrainValues[y * width + x];
}
FTerrainPoint TerrainData::GetFValueAt(int x, int y) const {
	if(x < 0 || x >= width) return {0, 0};
	if(y < 0 || y >= height) return {0, 0};

	return fTerrainValues[y * width + x];
}

// Constructors
TerrainData::TerrainData(int width, int height) : width(width), height(height) {}
TerrainData::TerrainData(int width, int height, vector<TerrainPoint>& terrainValues) : width(width), height(height), terrainValues(terrainValues) {
	fTerrainValues = vector<FTerrainPoint>(width * height);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			fTerrainValues[y * width + x].surfaceValue = (float)terrainValues[y * width + x].surfaceValue / 255.0f;
			fTerrainValues[y * width + x].oreValue = (float)terrainValues[y * width + x].oreValue / 255.0f;
		}
	}
}

TerrainData* TerrainData::ExtractRegion(int startx, int starty, int width, int height) const {
	vector<TerrainPoint> regionValues(width * height);
	vector<FTerrainPoint> fRegionValues(width * height);

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			regionValues[y * width + x] = GetValueAt(startx + x, starty + y);
			fRegionValues[y * width + x] = GetFValueAt(startx + x, starty + y);
		}
	}

	auto* terrainData = new TerrainData(width, height, regionValues);
	terrainData->id = TextFormat("%i %i", startx, starty);
	return terrainData;
}


// Getters
const int& TerrainData::GetHeight() const { return height; }
const int& TerrainData::GetWidth() const { return width; }
vector<TerrainPoint>& TerrainData::GetTerrainValues() { return terrainValues; }

// Setters
void TerrainData::SetTerrainValues(const vector<TerrainPoint>& terrainValues) {
	this->terrainValues = terrainValues;
}