#include "Terrain.h"

#include <iostream>

#include <cmath>

#include "config.h"

#include <rlgl.h>

#include "../../CoreGameLogic/AssetManager.h"
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

	terrainShader = AssetManager::LoadShader("Shaders/squareMarchingShader.frag");
	oreAtlas = AssetManager::LoadTexture("Textures/OreAtlas.png");
	oreColors = AssetManager::LoadTexture("Textures/OreColors.png");
	SetTextureWrap(oreAtlas, TEXTURE_WRAP_CLAMP);
	SetTextureWrap(oreColors, TEXTURE_WRAP_CLAMP);

	Vector2 screenSize(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());

	textureLoc = GetShaderLocation(terrainShader, "mapTexture");
	posLoc = GetShaderLocation(terrainShader, "position");
	int screenSizeLoc = GetShaderLocation(terrainShader, "screenSize");
	int surfaceLevelLoc = GetShaderLocation(terrainShader, "surfaceLevel");
	int terrainScaleLoc = GetShaderLocation(terrainShader, "terrainScale");
	int interpolationAmountLoc = GetShaderLocation(terrainShader, "interpolationAmount");
	int chunkSizeLoc = GetShaderLocation(terrainShader, "chunkSize");
	int chunkWorldSizeLoc = GetShaderLocation(terrainShader, "chunkWorldSize");
	atlasLoc = GetShaderLocation(terrainShader, "oreAtlas");
	oreColorsLoc = GetShaderLocation(terrainShader, "oreColors");

	SetShaderValue(terrainShader, screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(terrainShader, surfaceLevelLoc, &surfaceLevel, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, terrainScaleLoc, &worldScale, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, interpolationAmountLoc, &interpolationAmount, SHADER_UNIFORM_FLOAT);

	Vector2 chunkWorldSize(chunkSize * TerrainScale * worldScale, chunkSize * TerrainScale * worldScale);
	float floatChunkSize = chunkSize;
	SetShaderValue(terrainShader, chunkSizeLoc, &floatChunkSize, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, chunkWorldSizeLoc, &chunkWorldSize, SHADER_UNIFORM_VEC2);

	renderPass = RenderPass::Create(0, false);
	renderPass->AddFunction([this](RenderTexture2D& prev) {
		Render(prev);
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
			chunks[y][x]->Render(terrainShader, textureLoc, posLoc, atlasLoc, oreAtlas, oreColorsLoc, oreColors);
		}
	}
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

void Terrain::CheckChunkRaycast(Vector2 entryPos, Vector2 dir, float maxDistance, int cellX, int cellY, float unit, RaycastHitInfo& hitInfo) {
	// DrawCircle(entryPos.x, entryPos.y, 5, GREEN);
	if(cellX < 0) {
		hitInfo.hitPoint = entryPos;
		hitInfo.normal = {1, 0};
		hitInfo.hasHit = true;
		return;
	}
	if(cellX > instance->width) {
		hitInfo.hitPoint = entryPos;
		hitInfo.normal = {-1, 0};
		hitInfo.hasHit = true;
		return;
	}
	if(cellY < 0) {
		hitInfo.hitPoint = entryPos;
		hitInfo.normal = {0, 1};
		hitInfo.hasHit = true;
		return;
	}
	if(cellY > instance->height) {
		hitInfo.hitPoint = entryPos;
		hitInfo.normal = {0, -1};
		hitInfo.hasHit = true;
		return;
	}

	instance->chunks[cellY][cellX]->CheckRaycast(entryPos, dir, maxDistance, unit, instance->surfaceLevel, hitInfo);
}

/// @param dir - Normalized direction
RaycastHitInfo Terrain::Raycast(Vector2 origin, Vector2 dir, float maxDistance) {
	const float unit = instance->TerrainScale * instance->worldScale;
	int cellSize = instance->chunkSize * unit;
	float dist = 0;

	RaycastHitInfo res(0, false, origin, {0, 0}, {0, 0});

	// Amanatides-Woo fast voxel traversal algorithm
	int cellX = floor(origin.x / cellSize);
	int cellY = floor(origin.y / cellSize);

	int stepX, stepY;
	float mx, my;
	float dx, dy;

	if(dir.x > 0.0f) {
		stepX = 1;
		mx = ((cellX + 1) * cellSize - origin.x) / dir.x;
		dx = cellSize / dir.x;
	} else if(dir.x < 0.0f) {
		stepX = -1;
		mx = (origin.x - cellX * cellSize) / (-dir.x);
		dx = cellSize / (-dir.x);
	} else {
		stepX = 0;
		mx = numeric_limits<float>::infinity();
		dx = numeric_limits<float>::infinity();
	}

	if(dir.y > 0.0f) {
		stepY = 1;
		my = ((cellY + 1) * cellSize - origin.y) / dir.y;
		dy = cellSize / dir.y;
	} else if(dir.y < 0.0f) {
		stepY = -1;
		my = (origin.y - cellY * cellSize) / (-dir.y);
		dy = cellSize / (-dir.y);
	} else {
		stepY = 0;
		my = numeric_limits<float>::infinity();
		dy = numeric_limits<float>::infinity();
	}

	Vector2 entryPos = origin;
	CheckChunkRaycast(entryPos, dir, maxDistance, cellX, cellY, unit, res);
	if(res.hasHit) {
		if(res.hitDistance > maxDistance) {
			res.hasHit = false;
			res.hitPoint = origin + dir * maxDistance;
			res.normal = {0, 0};
		}
		return res;
	}

	while(res.hitDistance <= maxDistance) {
		if(mx < my) {
			res.hitDistance = mx;
			if(res.hitDistance > maxDistance) break;
			cellX += stepX;
			mx += dx;
		} else {
			res.hitDistance = my;
			if(res.hitDistance > maxDistance) break;
			cellY += stepY;
			my += dy;
		}
		entryPos = origin + dir * res.hitDistance;

		CheckChunkRaycast(entryPos, dir, maxDistance - res.hitDistance, cellX, cellY, unit, res);
		if(res.hasHit) {
			if(res.hitDistance > maxDistance) {
				res.hasHit = false;
				res.hitPoint = origin + dir * maxDistance;
				res.normal = {0, 0};
			}
			return res;
		}
	}

	return {-1, false, origin, {0, 0}, origin + dir * maxDistance};
}

void Terrain::Print(std::ostream &os) const {
	os << "Terrain";
}


float Terrain::GetSurfaceLevel() const {
	return surfaceLevel;
}


Terrain* Terrain::GetActiveTerrain() {
	return instance;
}
CollisionInfo Terrain::CheckCollisions(Vector2 pos, float radius) {
	float unit = instance->TerrainScale * instance->worldScale;

	radius /= unit;

	// Snap mouse position to nearest pixel
	int pixelX = round(pos.x / unit);
	int pixelY = round(pos.y / unit);

	// Get the chunk indices within the rectangle determined by the mining radius
	int bound_lx = pixelX - radius - 1, bound_rx = pixelX + radius;
	int bound_ly = pixelY - radius - 1, bound_ry = pixelY + radius;

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

	return res;
}


