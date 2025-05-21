#pragma once
#include "TerrainChunk.h"
#include "TerrainData.h"
#include "../../CoreGameLogic/Component.h"
#include "../../CoreGameLogic/RenderPass.h"
#include "../Collisions/CollisionInfo.h"
#include "../Collisions/RaycastHitInfo.h"
#include "../Ores/OreInfo.h"

class Terrain : public Component {
private:
	static Terrain* instance;

	int chunkSize;
	int width, height;
	TerrainChunk*** chunks; // 2D Array of chunk pointers
	const TerrainData* data;

	float surfaceLevel, worldScale, interpolationAmount;

	const float TerrainScale = 15.0;

	shared_ptr<RenderPass> renderPass;
	Shader terrainShader; int textureLoc, posLoc;

	int atlasLoc, oreColorsLoc, mapColorsLoc;
	Texture2D oreAtlas, oreColors, mapColors;

	void InitializeChunks();
	void Render(RenderTexture2D& prev);

protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	Terrain(const TerrainData* data, float surfaceLevel, float scale, float interpolationAmount, int chunkSize = 16);
	~Terrain() override;

	void UpdateSurfaceLevel(float newSurfaceLevel);

	OreInfo MineAt(Vector2 minePos, int radius, float miningPower, float deltaTime);

	float GetSurfaceLevel() const;

	static Terrain* GetActiveTerrain();
	// Only circle colliders will be supported
	static CollisionInfo CheckCollisions(Vector2 pos, float radius);

	static void CheckChunkRaycast(Vector2 entryPos, Vector2 dir, float maxDistance, int cellX, int cellY, float unit, RaycastHitInfo& hitInfo);
	static RaycastHitInfo Raycast(Vector2 origin, Vector2 dir, float maxDistance);

	void Print(std::ostream &os) const override;
};
