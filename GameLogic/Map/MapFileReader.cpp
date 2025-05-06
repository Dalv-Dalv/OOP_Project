#include "MapFileReader.h"

#include <cassert>
#include <iostream>

#include "../../Libraries/stb_image.h"
#include <vector>

#include "TerrainData.h"

using namespace std;

#define STBI_NO_STDIO //No writing, just reading

TerrainData* MapFileReader::ReadMap(const char* filepath) {
	int width, height, channels;
	unsigned char* data = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);
	if(!data) {
		throw runtime_error("Map file could not be read");
	}
	if(channels < 3) {
		throw runtime_error("Invalid map");
	}

	vector<TerrainPoint> map(width * height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = (y * width + x) * channels;
			float r = data[index + 0] / 255.0f;
			float g = data[index + 1] / 255.0f;
			float b = data[index + 2] / 255.0f;
			float a = data[index + 3] / 255.0f;

			map[(height - y - 1) * width + x] = {r, g, b, a};
		}
	}

	stbi_image_free(data);

	return new TerrainData(width, height, map);
}