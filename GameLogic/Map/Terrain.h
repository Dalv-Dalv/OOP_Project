#ifndef TERRAIN_H
#define TERRAIN_H
#include "TerrainChunk.h"
#include "TerrainData.h"
#include "../../CoreGameLogic/Component.h"

class Terrain : Component {
private:
	int chunkSize;
	int width, height;
	TerrainChunk*** chunks; // 2D Array of chunk pointers

	Shader cleanupShader; // Used to clean artificats left from square marching rendering

public:
	explicit Terrain(int chunkSize = 16);
	void Initialize(const TerrainData& map);

	~Terrain();

protected:
	void Awake() override;
	void Update(float deltaTime) override;
};

#endif //TERRAIN_H
