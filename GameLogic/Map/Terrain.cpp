#include "Terrain.h"

#include <iostream>

#include <cmath>

#include "config.h"
#include "../../cmake-build-debug/_deps/raylib-src/src/rlgl.h"
#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../../Utilities/Vector2Utils.h"
using namespace GameUtilities;

Terrain* Terrain::instance = nullptr;

Terrain::Terrain(const TerrainData* data, float surfaceLevel, float scale, float interpolationAmount, int chunkSize)
	: data(data), surfaceLevel(surfaceLevel), worldScale(scale), interpolationAmount(interpolationAmount), chunkSize(chunkSize) {
	if(instance != nullptr) {
		instance->gameObject->Destroy();
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

	width = (mapWidth + chunkSize - 1) / chunkSize;
	height = (mapHeight + chunkSize - 1) / chunkSize;

	chunks = new TerrainChunk**[height];
	for(int y = 0; y < height; y++) {
		chunks[y] = new TerrainChunk*[width];
		for(int x = 0; x < width; x++) {
			auto pos = Vector2(x * chunkSize * TerrainScale * worldScale, y * chunkSize * TerrainScale * worldScale);

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

	terrainRenderTexture = LoadRenderTexture(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
	cleanupRenderTexture = LoadRenderTexture(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());

	terrainShader = LoadShader(0, TextFormat("../Shaders/squareMarchingShader.frag"));
	cleanupShader = LoadShader(0, TextFormat("../Shaders/squareMarchingPostProcessingShader.frag", 430));


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
}


void Terrain::Update(float deltaTime) {
	Vector2 mousePos = GetMousePosition();
	mousePos.y = GameManager::GetWindowHeight() - mousePos.y;


	BeginTextureMode(terrainRenderTexture);
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
	//TODO: RENDER ONLY CHUNKS IN VIEW
				chunks[y][x]->Render(terrainShader, textureLoc, posLoc);
			}
		}

		// if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		// 	MineAt(mousePos, 4, 30.0, deltaTime);
		// }
	EndTextureMode();

	// Clean up artifacts left over by square marching shader
	// BeginTextureMode(cleanupRenderTexture);
	// 	ClearBackground(MAGENTA);
	// 	BeginShaderMode(cleanupShader);
	// 		Rectangle screenBounds(0, 0, GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
	// 		DrawTextureRec(terrainRenderTexture.texture,screenBounds,{0, 0},WHITE);
	// 	EndShaderMode();
	// EndTextureMode();

	GameManager::SetActiveRenderTexture(terrainRenderTexture);
}

void Terrain::OnGameClose() {
	UnloadShader(terrainShader);
	UnloadShader(cleanupShader);
}

void Terrain::UpdateSurfaceLevel(float newSurfaceLevel) {
	int surfaceLevelLoc = GetShaderLocation(terrainShader, "surfaceLevel");
	this->surfaceLevel = newSurfaceLevel;
	SetShaderValue(terrainShader, surfaceLevelLoc, &surfaceLevel, SHADER_UNIFORM_FLOAT);
}


void Terrain::MineAt(Vector2 minePos, float radius, float miningPower, float deltaTime) {
	DrawCircle(minePos.x, minePos.y, 3, RED);

	float unit = TerrainScale * worldScale;

	int pixelX = round(minePos.x / unit);
	int pixelY = round(minePos.y / unit);

	int bound_lx = pixelX - radius - 1, bound_rx = pixelX + radius;
	int bound_ly = pixelY - radius - 1, bound_ry = pixelY + radius;
	// DrawCircle((bound_lx + 1) * unit, (bound_ly + 1) * unit, 3, BLUE);
	// DrawCircle((bound_lx + 1) * unit, bound_ry * unit, 3, BLUE);
	// DrawCircle(bound_rx * unit, bound_ry * unit, 3, BLUE);
	// DrawCircle(bound_rx * unit, (bound_ly + 1) * unit, 3, BLUE);
	//
	// DrawCircle(pixelX * unit, pixelY * unit, 4, GREEN);

	bound_lx /= chunkSize; bound_rx /= chunkSize;
	bound_ly /= chunkSize; bound_ry /= chunkSize;

	for(int y = bound_ly; y <= bound_ry; y++) {
		for(int x = bound_lx; x <= bound_rx; x++) {
			int localX = pixelX - x * chunkSize;
			int localY = pixelY - y * chunkSize;
			localY = (chunkSize + 2) - localY;
			// DrawCircle(localX * unit, localY * unit, 3, MAGENTA);
			chunks[y][x]->MineAt(localX + 1, localY, radius, miningPower, deltaTime);
		}
	}
}

Terrain* Terrain::GetActiveTerrain() {
	return instance;
}

