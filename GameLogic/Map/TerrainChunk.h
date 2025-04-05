#pragma once

#include "TerrainData.h"
#include <raylib.h>

#include "../Collisions/CollisionInfo.h"

class TerrainChunk {
private:
	const Vector2 position;
	const int width, height; // World space width/height
	const int chunkWidth, chunkHeight;
	Texture2D gpuData; // Used for rendering
	TerrainData* cpuData; // Used for collisions

	static const vector<vector<int>> squareMarchingTable;

	void UpdateGPUData();
	float MiningFalloff(float radius, float distSqr);

	Vector2 CalculateEdgePoint(Vector2 v1, float w1, Vector2 v2, float w2, float surfaceLevel) const;
	void CheckMinCollisionAt(Vector2 pos, int posx, int posy, CollisionInfo& prevMin, float surfaceLevel, float unit) const;
public:
	TerrainChunk(Vector2 position, int width, int height, int chunkWidth, int chunkHeight, TerrainData* mapData);
	~TerrainChunk();

	// TODO: Possible Optimization (MUST MEASURE FIRST): each chunk with its own shader instance
	void Render(Shader& shader, int textureLoc, int posLoc);
	void MineAt(int posx, int posy, float radius, float miningPower, float deltaTime);

	CollisionInfo CheckCollisions(Vector2 pos, int posx, int posy, float radius, float surfaceLevel, float unit);

	void Highlight();

	const Vector2& GetPosition() const;
};
