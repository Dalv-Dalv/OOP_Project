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
	assert(data);

	unsigned char* map = new unsigned char[width * height];

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = (y * width + x) * channels;
			unsigned char r = data[index];

			map[y * width + x] = r;
		}
	}

	return new TerrainData(width, height, map);
}