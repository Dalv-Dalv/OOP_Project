#ifndef TERRAIN_H
#define TERRAIN_H
#include "TerrainChunk.h"
#include "TerrainData.h"
#include "../../CoreGameLogic/Component.h"
#include "../../CoreGameLogic/RenderPass.h"

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
	Shader terrainShader; int textureLoc, posLoc;
	Shader cleanupShader;

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

	static Terrain* GetActiveTerrain();
};

#endif //TERRAIN_H
