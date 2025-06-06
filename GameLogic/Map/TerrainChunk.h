#pragma once

#include <optional>

#include "TerrainData.h"
#include <raylib.h>

#include "../Collisions/CollisionInfo.h"
#include "../Collisions/RaycastHitInfo.h"
#include "../Ores/OreInfo.h"

class TerrainChunk {
private:
	const Vector2 position;
	const int width, height; // World space width/height
	const int chunkWidth, chunkHeight;
	Texture2D gpuData; // Used for rendering
	TerrainData* cpuData; // Used for collisions

	static Sound orePickupSound;

	static const vector<vector<int>> squareMarchingTable;

	void UpdateGPUData();
	float MiningFalloff(float radius, float distSqr);

	Vector2 CalculateEdgePoint(Vector2 v1, float w1, Vector2 v2, float w2, float surfaceLevel) const;
	void CheckMinCollisionAt(Vector2 pos, int posx, int posy, CollisionInfo& prevMin, float surfaceLevel, float unit) const;
public:
	TerrainChunk(Vector2 position, int width, int height, int chunkWidth, int chunkHeight, TerrainData* mapData);
	~TerrainChunk();

	// TODO: Possible Optimization (MUST MEASURE FIRST): each chunk with its own shader instance
	void Render(Shader& shader, int textureLoc, int posLoc, int atlasLoc, const Texture2D& oreAtlas, int oreColorsLoc, const Texture2D& oreColors, int mapColorsLoc, const Texture2D& mapColors);
	void MineAt(int posx, int posy, float radius, float miningPower, float deltaTime, OreInfo& miningResult);

	CollisionInfo CheckCollisions(Vector2 pos, int posx, int posy, float radius, float surfaceLevel, float unit);
	void RaySegmentIntersection(Vector2 rOrigin, Vector2 rDir, Vector2 p1, Vector2 p2, RaycastHitInfo& hitInfo);
	void CheckRay(Vector2 entryPos, Vector2 dir, int cellX, int cellY, float unit, float surfaceLevel, RaycastHitInfo& hitInfo);
	void CheckRaycast(Vector2 origin, Vector2 dir, float maxDistance, float unit, float surfaceLevel, RaycastHitInfo& hitInfo);

	void Highlight();

	const Vector2& GetPosition() const;
};
