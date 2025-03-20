#include "TerrainData.h"

#include <iostream>

void TerrainData::SetValueAt(int x, int y, uint8_t value) {
	if(x < 0 || x > width) return;
	if(y < 0 || y > height) return;

	flattenedValues[y * width + x] = value;
}

uint8_t TerrainData::GetValueAt(int x, int y) const {
	if(x < 0 || x > width) return 0;
	if(y < 0 || y > height) return 0;

	return flattenedValues[y * width + x];
}

// Constructors
TerrainData::TerrainData(int width, int height) : width(width), height(height) {}
TerrainData::TerrainData(int width, int height, const vector<uint8_t>& flattenedValues) : width(width), height(height), flattenedValues(flattenedValues) {}

const TerrainData TerrainData::ExtractRegion(int startx, int starty, int width, int height) const {
	vector<uint8_t> regionValues(width * height);
	TerrainData terrainData(width, height, regionValues);

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			terrainData.SetValueAt(x, y, GetValueAt(startx + x, starty + y));
		}
	}

	return terrainData;
}


// Getters
const int& TerrainData::GetHeight() const { return height; }
const int& TerrainData::GetWidth() const { return width; }
const vector<uint8_t>& TerrainData::GetFlattenedValues() const { return flattenedValues; }

// Setters
void TerrainData::SetFlattenedValues(const vector<uint8_t>& flattenedValues) {
	this->flattenedValues = flattenedValues;
}
