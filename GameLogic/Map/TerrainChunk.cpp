#include "TerrainChunk.h"

#include <iostream>

#include "../../cmake-build-release/_deps/raylib-src/src/raymath.h"
#include "../../Utilities/GameUtilities.h"

void TerrainChunk::UpdateGPUData() {
	auto imageData = cpuData->GetFlattenedValues();
	UpdateTexture(gpuData, imageData);
}

float TerrainChunk::MiningFalloff(float radius, float distSqr, float miningPower) {
	if(distSqr > radius * radius || distSqr < 0) return 0;

	distSqr = sqrt(distSqr);

	// linear
	float t = GameUtilities::inverseLerp(0, radius, distSqr);

	// quadratic
	t = 1 - t * t * t;
	return t * miningPower;
}


TerrainChunk::TerrainChunk(Vector2 position, int width, int height, int chunkWidth, int chunkHeight, TerrainData* mapData)
: position(position), width(width), height(height), chunkWidth(chunkWidth), chunkHeight(chunkHeight), cpuData(mapData){
	unsigned char* imageData = mapData->GetFlattenedValues();

	Image mapImage {
		.data = imageData,
		.width = mapData->GetWidth(),
		.height = mapData->GetHeight(),
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE
	};

	gpuData = LoadTextureFromImage(mapImage);
	// UnloadImage(mapImage); UNCOMMENTING THIS MAKES CHUNKS SHARE mapData ADDRESS FOR ??SOME REASON??
}
TerrainChunk::~TerrainChunk() {
	delete cpuData;
}


void TerrainChunk::Render(Shader& shader, int textureLoc, int posLoc) {
	BeginShaderMode(shader);
		SetShaderValueTexture(shader, textureLoc, gpuData);
		SetShaderValue(shader, posLoc, &position, SHADER_UNIFORM_VEC2);
		DrawRectangle(position.x, position.y, width + 1, height + 1, WHITE);
	EndShaderMode();
}

void TerrainChunk::MineAt(int posx, int posy, float radius, float miningPower, float deltaTime) {
	for(int x = posx - radius; x < posx + radius; x++) {
		if(x < 0) continue;
		if(x > chunkWidth) break;

		for(int y = posy - radius; y < posy + radius; y++) {
			if(y < 0) continue;
			if(y > chunkHeight) break;

			// Distance squared
			float distSqr = (posx - x) * (posx - x) + (posy - y) * (posy - y);

			float val = cpuData->GetValueAt(x, y);
			if(miningPower * deltaTime > val) val = 0;
			else val -= miningPower * deltaTime;
			cpuData->SetValueAt(x, y, val);
		}
	}

	UpdateGPUData();
}

void TerrainChunk::Highlight() {
	DrawRectangle(position.x, position.y, width + 1, height + 1, Color(0, 0, 255, 100));
}



// Getters
const Vector2& TerrainChunk::GetPosition() const { return position; }