#ifndef MAP_H
#define MAP_H
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class TerrainData {
private:
	int width, height; // Map width/height
	vector<uint8_t> flattenedValues; // Actually a 2D map

public:
	void SetValueAt(int x, int y, uint8_t value);
	uint8_t GetValueAt(int x, int y) const;

	TerrainData(int width, int height);
	TerrainData(int width, int height, const vector<uint8_t>& flattenedValues);

	const TerrainData ExtractRegion(int startx, int starty, int width, int height) const;

	const int& GetWidth() const;
	const int& GetHeight() const;
	const vector<uint8_t>& GetFlattenedValues() const;
	void SetFlattenedValues(const vector<uint8_t>& flattenedValues);
};

#endif //MAP_H
