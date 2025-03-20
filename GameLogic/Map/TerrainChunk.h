#ifndef TERRAINCHUNK_H
#define TERRAINCHUNK_H
#include "TerrainData.h"
#include "raylib.h"

class TerrainChunk {
private:
	const Vector2 position;
	const int width, height; // World space width/height
	Texture2D gpuData; // Used for rendering
	TerrainData cpuData; // Used for collisions

	void UpdateGPUData();
public:
	TerrainChunk(Vector2 position, int width, int height, const TerrainData& mapData);

	// TODO: Possible Optimization (MUST MEASURE FIRST): each chunk with its own shader instance
	void Render(Shader shader, int posLoc, int texLoc) const;

	const Vector2& GetPosition() const;
};

#endif //TERRAINCHUNK_H