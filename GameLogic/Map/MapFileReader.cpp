#include "MapFileReader.h"

#include <cassert>
#include <iostream>

#include "../../Libraries/stb_image.h"
#include <vector>

#include "TerrainData.h"

using namespace std;

TerrainData* MapFileReader::ReadMap(const char* filepath) {
	int width, height, channels;
	unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
	if(!data) {
		throw runtime_error("Map file could not be read");
	}
	if(channels != 3) {
		throw runtime_error("Invalid map");
	}

	vector<TerrainPoint> map(width * height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = (y * width + x) * channels;
			unsigned char r = data[index + 0];
			unsigned char g = data[index + 1];
			unsigned char b = data[index + 2];

			map[y * width + x] = {r, g, b};
		}
	}

	stbi_image_free(data);

	return new TerrainData(width, height, map);
}