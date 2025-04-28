#pragma once

// For displaying, has 0-255 values
struct TerrainPoint {
	unsigned char surfaceValue, oreValue, oreType;

	TerrainPoint() : surfaceValue(0), oreValue(0), oreType(0) {}
	TerrainPoint(unsigned char surfaceLevel, unsigned char oreValue, unsigned char oreType)
		: surfaceValue(surfaceLevel), oreValue(oreValue), oreType(oreType) {}
};

// For mining, has 0-1 float values
struct FTerrainPoint {
	float surfaceValue, oreValue;

	FTerrainPoint() : surfaceValue(0), oreValue(0) {}
	FTerrainPoint(float surfaceLevel, float oreValue)
		: surfaceValue(surfaceLevel), oreValue(oreValue) {}
};
