#include "Terrain.h"

#include <iostream>

#include <cmath>

#include "config.h"
#include "../../cmake-build-debug/_deps/raylib-src/src/rlgl.h"
#include "../../CoreGameLogic/GameManager.h"
#include "../../Utilities/Vector2Utils.h"
using namespace GameUtilities;

Terrain::Terrain(const TerrainData* data, float surfaceLevel, float scale, float interpolationAmount, int chunkSize)
	: data(data), surfaceLevel(surfaceLevel), worldScale(scale), interpolationAmount(interpolationAmount), chunkSize(chunkSize) {}

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

void Terrain::MineAt(Vector2 minePos, float radius, float miningPower, float deltaTime) {
	float unit = TerrainScale * worldScale;

	int pixelX = round(minePos.x / unit);
	int pixelY = round(minePos.y / unit);

	DrawCircle(pixelX * unit, pixelY * unit, 4, GREEN);

	pixelX %= chunkSize + 1;
	pixelY %= chunkSize + 1;
	pixelY = chunkSize - pixelY;
	pixelX += 1;

	DrawCircle(pixelX * unit, pixelY * unit, 4, DARKGREEN);


	DrawCircle(minePos.x, minePos.y, 3, RED);
	int posx = round(minePos.x / (unit));
	int posy = round(minePos.y / (unit));
	int bound_lx = posx - radius, bound_rx = posx + radius;
	int bound_ly = posy - radius, bound_ry = posy + radius;
	DrawCircle(bound_lx * unit, bound_ly * unit, 3, BLUE);
	DrawCircle(bound_lx * unit, bound_ry * unit, 3, BLUE);
	DrawCircle(bound_rx * unit, bound_ry * unit, 3, BLUE);
	DrawCircle(bound_rx * unit, bound_ly * unit, 3, BLUE);

	// chunks[posy][posx]->Highlight();
	// chunks[posy][posx]->MineAt(pixelX, pixelY, radius, miningPower, deltaTime);
}


void Terrain::Awake() {
	InitializeChunks();

	terrainRenderTexture = LoadRenderTexture(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
	cleanupRenderTexture = LoadRenderTexture(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());

	terrainShader = LoadShader(0, TextFormat("../Shaders/squareMarchingShader.fs"));
	cleanupShader = LoadShader(0, TextFormat("../Shaders/squareMarchingPostProcessingShader.fs", 430));


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
	//TODO: Get the chunks that are in view and call Render()

	Vector2 mousePos = GetMousePosition();
	mousePos.y = GameManager::GetWindowHeight() - mousePos.y;
	int mouseLoc = GetShaderLocation(terrainShader, "mousePos");
	SetShaderValue(terrainShader, mouseLoc, &mousePos, SHADER_UNIFORM_VEC2);

	int timeLoc = GetShaderLocation(terrainShader, "time");
	float time = GetTime();
	SetShaderValue(terrainShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

	BeginTextureMode(terrainRenderTexture);
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				chunks[y][x]->Render(terrainShader, textureLoc, posLoc);
			}
		}

		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			MineAt(mousePos, 1, 0.3, deltaTime);
		}
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

	//TODO: After rendering each chunk, render square marching post processing shader
}

void Terrain::OnGameClose() {
	UnloadShader(terrainShader);
	UnloadShader(cleanupShader);
}




