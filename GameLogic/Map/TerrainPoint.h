#pragma once

// Has 0-1 floating point values
struct TerrainPoint {
	float surfaceValue, oreValue, oreType;

	TerrainPoint() : surfaceValue(0), oreValue(0), oreType(0) {}
	TerrainPoint(float surfaceLevel, float oreValue, float oreType)
		: surfaceValue(surfaceLevel), oreValue(oreValue), oreType(oreType) {}
};