#include "MapFileReader.h"

#include <cassert>
#include <iostream>

#include "../../Libraries/stb_image.h"
#include <vector>

using namespace std;

vector<vector<float>> MapFileReader::ReadMap(const char* filepath) {
	vector<vector<float>> map;
	int width, height, channels;
	unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);

	assert(data);

	map.resize(height, vector(width, 0.0f));

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = (y * width + x) * channels;
			unsigned char r = data[index];

			map[y][x] = static_cast<float>(r) / 255.0f;
		}
	}

	return map;
}