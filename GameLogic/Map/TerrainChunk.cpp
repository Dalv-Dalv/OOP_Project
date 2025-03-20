#include "TerrainChunk.h"

#include <iostream>

void TerrainChunk::UpdateGPUData() {
	auto v = cpuData.GetFlattenedValues();
	UpdateTexture(gpuData, v.data());
}

TerrainChunk::TerrainChunk(Vector2 position, int width, int height, const TerrainData& mapData)
: position(position), width(width), height(height), cpuData(mapData){
	unsigned char* imageData = mapData.GetFlattenedValues();
	Image mapImage = {
		.data = imageData,
		.width = mapData.GetWidth(),
		.height = mapData.GetHeight(),
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE
	};

	gpuData = LoadTextureFromImage(mapImage);
	UnloadImage(mapImage);

	cout << "CONSTRUCTOR DONE???????\n";
}

void TerrainChunk::Render(Shader shader, int posLoc, int texLoc) const {
	// Shader mode is assumed to be already started
	SetShaderValue(shader, posLoc, &position, SHADER_UNIFORM_VEC2);
	SetShaderValueTexture(shader, texLoc, gpuData);

	DrawRectangle(position.x, position.y, width, height, WHITE);
}


// Getters
const Vector2& TerrainChunk::GetPosition() const { return position; }