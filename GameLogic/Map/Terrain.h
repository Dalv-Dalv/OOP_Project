#ifndef TERRAIN_H
#define TERRAIN_H
#include "TerrainChunk.h"
#include "TerrainData.h"
#include "../../CoreGameLogic/Component.h"

class Terrain : public Component {
private:
	static Terrain* instance;

	int chunkSize;
	int width, height;
	TerrainChunk*** chunks; // 2D Array of chunk pointers
	const TerrainData* data;

	float surfaceLevel, worldScale, interpolationAmount;

	const float TerrainScale = 15.0;

	RenderTexture2D terrainRenderTexture, cleanupRenderTexture;
	Shader terrainShader; int textureLoc, posLoc;
	Shader cleanupShader; // Used to clean artificats left from square marching rendering

	void InitializeChunks();
	void GetChunkPosFromWorldPos(Vector2 worldPos);

protected:
	void Awake() override;
	void Update(float deltaTime) override;
	void OnGameClose() override;

public:
	Terrain(const TerrainData* data, float surfaceLevel, float scale, float interpolationAmount, int chunkSize = 16);
	~Terrain();

	void UpdateSurfaceLevel(float newSurfaceLevel);

	void MineAt(Vector2 minePos, float radius, float miningPower, float deltaTime);

	static Terrain* GetActiveTerrain();
};

#endif //TERRAIN_H
