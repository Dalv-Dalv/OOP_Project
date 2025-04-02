#pragma once
#include "TerrainData.h"
#include <raylib.h>

class TerrainChunk {
private:
	const Vector2 position;
	const int width, height; // World space width/height
	const int chunkWidth, chunkHeight;
	Texture2D gpuData; // Used for rendering
	TerrainData* cpuData; // Used for collisions

	void UpdateGPUData();
	float MiningFalloff(float radius, float distSqr);
public:
	TerrainChunk(Vector2 position, int width, int height, int chunkWidth, int chunkHeight, TerrainData* mapData);
	~TerrainChunk();

	// TODO: Possible Optimization (MUST MEASURE FIRST): each chunk with its own shader instance
	void Render(Shader& shader, int textureLoc, int posLoc);
	void MineAt(int posx, int posy, float radius, float miningPower, float deltaTime);

	void Highlight();

	const Vector2& GetPosition() const;
};