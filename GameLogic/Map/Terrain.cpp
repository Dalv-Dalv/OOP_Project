#include "Terrain.h"

#include <iostream>

#include "config.h"
#include "../../CoreGameLogic/GameManager.h"

Terrain::~Terrain() {
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			delete chunks[y][x];
		}
		delete[] chunks[y];
 	}
	delete[] chunks;
}

Terrain::Terrain(int chunkSize) : chunkSize(chunkSize) {}

void Terrain::Initialize(const TerrainData& map) {
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	width = (mapWidth + chunkSize - 1) / chunkSize;
	height = (mapHeight + chunkSize - 1) / chunkSize;

	chunks = new TerrainChunk**[height];
	for(int y = 0; y < height; y++) {
		chunks[y] = new TerrainChunk*[width];
		for(int x = 0; x < width; x++) {
			auto pos = Vector2(x, y);

			cout<<"hello? " << endl;
			TerrainData chunkData = map.ExtractRegion(x * chunkSize, y * chunkSize, chunkSize, chunkSize);
			cout<<"hi" << endl;

			chunks[y][x] = new TerrainChunk(pos, chunkSize, chunkSize, chunkData);
			cout << "there" << endl;
		}
	}
}

void Terrain::Awake() {
	cleanupShader = LoadShader(0, TextFormat("../Shaders/squareMarchingPostProcessingShader.fs", 430));

	int screenSizeLoc = GetShaderLocation(cleanupShader, "screenSize");
	Vector2 screenSize(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
	SetShaderValue(cleanupShader, screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
}


void Terrain::Update(float deltaTime) {
	//TODO: Get the chunks that are in view and call Render()

	//TODO: After rendering each chunk, render square marching post processing shader
}



