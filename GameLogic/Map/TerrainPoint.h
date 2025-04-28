#pragma once

struct TerrainPoint {
	unsigned char surfaceValue, oreValue, oreType;

	TerrainPoint()
		: surfaceValue(0), oreValue(0), oreType(0) {}

	TerrainPoint(unsigned char surfaceLevel, unsigned char oreValue, unsigned char oreType)
		: surfaceValue(surfaceLevel), oreValue(oreValue), oreType(oreType) {
	}
};
