#ifndef MAP_H
#define MAP_H
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class TerrainData {
private:
	int width, height; // Map width/height
	unsigned char* flattenedValues; // Actually a 2D map

public:
	string id;
	void SetValueAt(int x, int y, unsigned char value);
	unsigned char GetValueAt(int x, int y) const;

	TerrainData(int width, int height);
	TerrainData(int width, int height, unsigned char* flattenedValues);

	TerrainData* ExtractRegion(int startx, int starty, int width, int height) const;

	const int& GetWidth() const;
	const int& GetHeight() const;
	unsigned char* GetFlattenedValues();
	void SetFlattenedValues(unsigned char* flattenedValues);
};

#endif //MAP_H
