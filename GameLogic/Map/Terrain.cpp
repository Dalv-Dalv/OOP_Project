#include "Terrain.h"

#include <iostream>

#include <cmath>

#include "config.h"

#include <rlgl.h>
#include <raymath.h>

#include "../PlayerMovement.h"
#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../../Utilities/Vector2Utils.h"
using namespace GameUtilities;

Terrain* Terrain::instance = nullptr;

Terrain::Terrain(const TerrainData* data, float surfaceLevel, float scale, float interpolationAmount, int chunkSize)
	: data(data), surfaceLevel(surfaceLevel), worldScale(scale), interpolationAmount(interpolationAmount), chunkSize(chunkSize) {
	if(instance != nullptr) {
		delete instance->gameObject;
	}
	instance = this;
}

Terrain::~Terrain() {
	UnloadShader(terrainShader);
	UnloadShader(cleanupShader);

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			delete chunks[y][x];
		}
		delete[] chunks[y];
 	}
	delete[] chunks;

	// In case InitializeChunks() hasnt been called
	if(data != nullptr) delete data;
}


void Terrain::InitializeChunks() {
	int mapWidth = data->GetWidth();
	int mapHeight = data->GetHeight();

	// Determine how many chunks we need to make sure we can fit the whole map
	width = (mapWidth + chunkSize - 1) / chunkSize;
	height = (mapHeight + chunkSize - 1) / chunkSize;

	chunks = new TerrainChunk**[height];
	for(int y = 0; y < height; y++) {
		chunks[y] = new TerrainChunk*[width];
		for(int x = 0; x < width; x++) {
			auto pos = Vector2(x * chunkSize * TerrainScale * worldScale, y * chunkSize * TerrainScale * worldScale);

			// Get a region of the map to pass to the chunk
			TerrainData* chunkData = data->ExtractRegion(x * chunkSize - 1, (height - y - 1) * chunkSize - 1, chunkSize + 2, chunkSize + 2);

			chunks[y][x] = new TerrainChunk(pos, chunkSize * TerrainScale * worldScale, chunkSize * TerrainScale * worldScale, chunkSize + 2, chunkSize + 2, chunkData);
		}
	}

	// No use for the old outdated data
	delete data;
	data = nullptr;
}


void Terrain::Awake() {
	InitializeChunks();

	terrainShader = LoadShader(0, TextFormat("Shaders/squareMarchingShader.frag"));
	cleanupShader = LoadShader(0, TextFormat("Shaders/squareMarchingPostProcessingShader.frag", 430));

	int screenSizeLoc = GetShaderLocation(cleanupShader, "screenSize");
	Vector2 screenSize(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
	SetShaderValue(cleanupShader, screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);

	screenSizeLoc = GetShaderLocation(terrainShader, "screenSize");
	textureLoc = GetShaderLocation(terrainShader, "mapTexture");
	posLoc = GetShaderLocation(terrainShader, "position");
	int surfaceLevelLoc = GetShaderLocation(terrainShader, "surfaceLevel");
	int terrainScaleLoc = GetShaderLocation(terrainShader, "terrainScale");
	int interpolationAmountLoc = GetShaderLocation(terrainShader, "interpolationAmount");
	int chunkSizeLoc = GetShaderLocation(terrainShader, "chunkSize");
	int chunkWorldSizeLoc = GetShaderLocation(terrainShader, "chunkWorldSize");

	SetShaderValue(terrainShader, screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(terrainShader, surfaceLevelLoc, &surfaceLevel, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, terrainScaleLoc, &worldScale, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, interpolationAmountLoc, &interpolationAmount, SHADER_UNIFORM_FLOAT);

	Vector2 chunkWorldSize(chunkSize * TerrainScale * worldScale, chunkSize * TerrainScale * worldScale);
	float floatChunkSize = chunkSize;
	SetShaderValue(terrainShader, chunkSizeLoc, &floatChunkSize, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, chunkWorldSizeLoc, &chunkWorldSize, SHADER_UNIFORM_VEC2);

	renderPass = RenderPass::Create(GameManager::GetWindowWidth(), GameManager::GetWindowHeight(), 0, false);
	renderPass->AddFunction([this](RenderTexture2D& prev) {
		Render(prev);
	});

	cleanupPass = RenderPass::Create(GameManager::GetWindowWidth(), GameManager::GetWindowHeight(), 1, false);
	cleanupPass->AddFunction([this](RenderTexture2D& prev) {
		CleanupRender(prev);
	});
}


void Terrain::Update(float deltaTime) {}

void Terrain::Render(RenderTexture2D& prev) {
	// Render only the chunks in view
	auto rect = GameCamera::GetActiveCamera()->GetWorldSpaceScreenRect();
	float unit = TerrainScale * worldScale;

	int pixelX = floor(rect.x / unit);
	int pixelY = floor(rect.y / unit);

	// Get the chunk indices within the rectangle determined by the rectangle bounds
	int bound_lx = pixelX;
	int bound_rx = pixelX + (rect.width + unit - 1) / unit;
	int bound_ly = pixelY;
	int bound_ry = pixelY + (rect.height + unit - 1) / unit;

	bound_lx /= chunkSize; bound_rx /= chunkSize;
	bound_ly /= chunkSize; bound_ry /= chunkSize;

	bound_lx = max(0, bound_lx);
	bound_ly = max(0, bound_ly);
	bound_rx = min(bound_rx, width - 1);
	bound_ry = min(bound_ry, height - 1);

	RenderPipeline::DrawTextureFullScreen(prev);

	for(int y = bound_ly; y <= bound_ry; y++) {
		for(int x = bound_lx; x <= bound_rx; x++) {
			chunks[y][x]->Render(terrainShader, textureLoc, posLoc);
		}
	}
}
void Terrain::CleanupRender(RenderTexture2D& prev) {
	BeginShaderMode(cleanupShader);
		RenderPipeline::DrawTextureFullScreen(prev);
	EndShaderMode();
}



void Terrain::UpdateSurfaceLevel(float newSurfaceLevel) {
	int surfaceLevelLoc = GetShaderLocation(terrainShader, "surfaceLevel");
	this->surfaceLevel = newSurfaceLevel;
	SetShaderValue(terrainShader, surfaceLevelLoc, &surfaceLevel, SHADER_UNIFORM_FLOAT);
}

void Terrain::MineAt(Vector2 minePos, int radius, float miningPower, float deltaTime) {
	float unit = TerrainScale * worldScale;

	// Snap mouse position to nearest pixel
	int pixelX = round(minePos.x / unit);
	int pixelY = round(minePos.y / unit);

	// Get the chunk indices within the rectangle determined by the mining radius
	int bound_lx = pixelX - radius - 1, bound_rx = pixelX + radius;
	int bound_ly = pixelY - radius - 1, bound_ry = pixelY + radius;

	bound_lx /= chunkSize; bound_rx /= chunkSize;
	bound_ly /= chunkSize; bound_ry /= chunkSize;

	bound_lx = max(0, bound_lx); bound_ly = max(0, bound_ly);
	bound_rx = min(bound_rx, width - 1); bound_ry = min(bound_ry, height - 1);

	for(int y = bound_ly; y <= bound_ry; y++) {
		for(int x = bound_lx; x <= bound_rx; x++) {
			// Calculate the local pixel coordinates for the chunk to use
			int localX = pixelX - x * chunkSize;
			int localY = pixelY - y * chunkSize;
			localY = (chunkSize + 2) - localY;

			chunks[y][x]->MineAt(localX + 1, localY, radius, miningPower, deltaTime);
		}
	}
}


void Terrain::Print(std::ostream &os) const {
	os << "Terrain";
}


Terrain* Terrain::GetActiveTerrain() {
	return instance;
}
CollisionInfo Terrain::CheckCollisions(Vector2 pos, float radius) {
	float unit = instance->TerrainScale * instance->worldScale;

	radius = 200;

	radius /= unit;

	// Snap mouse position to nearest pixel
	int pixelX = round(pos.x / unit);
	int pixelY = round(pos.y / unit);

	DrawCircle(pixelX * unit, pixelY * unit, 10, RED);

	// Get the chunk indices within the rectangle determined by the mining radius
	int bound_lx = pixelX - radius - 1, bound_rx = pixelX + radius;
	int bound_ly = pixelY - radius - 1, bound_ry = pixelY + radius;

	DrawCircle(bound_lx * unit, bound_ly * unit, 10, PURPLE);
	DrawCircle(bound_lx * unit, bound_ry * unit, 10, PURPLE);
	DrawCircle(bound_rx * unit, bound_ry * unit, 10, PURPLE);
	DrawCircle(bound_rx * unit, bound_ly * unit, 10, PURPLE);

	bound_lx /= instance->chunkSize; bound_rx /= instance->chunkSize;
	bound_ly /= instance->chunkSize; bound_ry /= instance->chunkSize;

	bound_lx = max(0, bound_lx); bound_ly = max(0, bound_ly);
	bound_rx = min(bound_rx, instance->width - 1); bound_ry = min(bound_ry, instance->height - 1);


	CollisionInfo res(1000000000, {0, 0}, pos);
	for(int y = bound_ly; y <= bound_ry; y++) {
		for(int x = bound_lx; x <= bound_rx; x++) {
			// Calculate the local pixel coordinates for the chunk to use
			int localX = pixelX - x * instance->chunkSize;
			int localY = pixelY - y * instance->chunkSize;
			localY = (instance->chunkSize + 2) - localY;

			res = min(res, instance->chunks[y][x]->CheckCollisions(pos, localX, localY, radius, instance->surfaceLevel, unit));
		}
	}

	DrawLineEx(pos, res.closestPoint, 3, GREEN);
	DrawCircle(res.closestPoint.x, res.closestPoint.y, 7, GREEN);

	return res;
}


