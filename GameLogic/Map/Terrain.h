#pragma once
#include "TerrainChunk.h"
#include "TerrainData.h"
#include "../../CoreGameLogic/Component.h"
#include "../../CoreGameLogic/RenderPass.h"
#include "../Collisions/CollisionInfo.h"

class Terrain : public Component {
private:
	static Terrain* instance;

	int chunkSize;
	int width, height;
	TerrainChunk*** chunks; // 2D Array of chunk pointers
	const TerrainData* data;

	float surfaceLevel, worldScale, interpolationAmount;

	const float TerrainScale = 15.0;

	shared_ptr<RenderPass> renderPass; unsigned int renderID;
	shared_ptr<RenderPass> cleanupPass; unsigned int cleanupRenderID;
	Shader terrainShader; int textureLoc, posLoc, atlasLoc, oreColorsLoc;
	Shader cleanupShader;

	Texture2D oreAtlas, oreColors;

	void InitializeChunks();
	void Render(RenderTexture2D& prev);
	void CleanupRender(RenderTexture2D& prev);

protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	Terrain(const TerrainData* data, float surfaceLevel, float scale, float interpolationAmount, int chunkSize = 16);
	~Terrain();

	void UpdateSurfaceLevel(float newSurfaceLevel);

	void MineAt(Vector2 minePos, int radius, float miningPower, float deltaTime);

	float GetSurfaceLevel() const;

	static Terrain* GetActiveTerrain();
	// Only circle colliders will be supported
	static CollisionInfo CheckCollisions(Vector2 pos, float radius);

	void Print(std::ostream &os) const override;
};