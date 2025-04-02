#include "TerrainData.h"

#include <iostream>

#include "../../cmake-build-release/_deps/raylib-src/src/raylib.h"

void TerrainData::SetValueAt(int x, int y, unsigned char value) {
	if(x < 0 || x > width) return;
	if(y < 0 || y > height) return;

	flattenedValues[y * width + x] = value;
}

unsigned char TerrainData::GetValueAt(int x, int y) const {
	if(x < 0 || x > width) return 0;
	if(y < 0 || y > height) return 0;

	return flattenedValues[y * width + x];
}

// Constructors
TerrainData::TerrainData(int width, int height) : width(width), height(height) {}
TerrainData::TerrainData(int width, int height, vector<unsigned char>& flattenedValues) : width(width), height(height), flattenedValues(flattenedValues) {}

TerrainData* TerrainData::ExtractRegion(int startx, int starty, int width, int height) const {
	vector<unsigned char> regionValues(width * height);

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			regionValues[y * width + x] = GetValueAt(startx + x, starty + y);
		}
	}

	auto* terrainData = new TerrainData(width, height, regionValues);
	terrainData->id = TextFormat("%i %i", startx, starty);
	return terrainData;
}


// Getters
const int& TerrainData::GetHeight() const { return height; }
const int& TerrainData::GetWidth() const { return width; }
vector<unsigned char>& TerrainData::GetFlattenedValues() { return flattenedValues; }

// Setters
void TerrainData::SetFlattenedValues(vector<unsigned char>& flattenedValues) {
	this->flattenedValues = flattenedValues;
}
