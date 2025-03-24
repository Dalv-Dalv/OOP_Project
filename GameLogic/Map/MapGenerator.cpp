#include "MapGenerator.h"

#include <iostream>

#include "raylib.h"
#include "../../Utilities/GameUtilities.h"
#include "../../Utilities/Vector2Utils.h"

using namespace GameUtilities;

// Cases from https://ragingnexus.com/creative-code-lab/experiments/algorithms-marching-squares/
vector<char> MapGenerator::caseIndexToEdges[] = {
	/* Case  0 */{},
	/* Case  1 */{static_cast<char>(0b10001110)},
	/* Case  2 */{static_cast<char>(0b11100111)},
	/* Case  3 */{static_cast<char>(0b10000111)},
	/* Case  4 */{static_cast<char>(0b01000111)},
	/* Case  5 */{static_cast<char>(0b10000100), static_cast<char>(0b11100111)},
	/* Case  6 */{static_cast<char>(0b11100100)},
	/* Case  7 */{static_cast<char>(0b10000100)},
	/* Case  8 */{static_cast<char>(0b10000100)},
	/* Case  9 */{static_cast<char>(0b11100100)},
	/* Case 10 */{static_cast<char>(0b01000111), static_cast<char>(0b10001110)},
	/* Case 11 */{static_cast<char>(0b01000111)},
	/* Case 12 */{static_cast<char>(0b10000111)},
	/* Case 13 */{static_cast<char>(0b11100111)},
	/* Case 14 */{static_cast<char>(0b10001110)},
	/* Case 15 */{}
};

pair<Vector2, Vector2> MapGenerator::CalculateEdge(const float surfaceLevel, const float interpolationLevel, const char lineIndex) {
	Vector2 a, b; // Points between which to calculate a middle point
	float va, vb; // Surface level at points A and B
	Vector2 from, to;
	a.x = (lineIndex & 0b0001) >> 0;
	a.y = (lineIndex & 0b0010) >> 1;
	b.x = (lineIndex & 0b0100) >> 2;
	b.y = (lineIndex & 0b1000) >> 3;

	// float t2 = 0.5;
	// float t1 = (surfaceLevel - v1.z) / (v2.z - v1.z);
	// float t = t1 + (t2 - t1) * interpolationLevel;
	float t = 0.5;
	from = GameUtilities::lerp(a, b, t);

	a.x = (lineIndex & 0b00010000) >> 4;
	a.y = (lineIndex & 0b00100000) >> 5;
	b.x = (lineIndex & 0b01000000) >> 6;
	b.y = (lineIndex & 0b10000000) >> 7;

	to = GameUtilities::lerp(a, b, t);

	return {from, to};
}


vector<pair<Vector2, Vector2>> MapGenerator::GenerateLineList(const float surfaceLevel, const float interpolationAmount, const float mapScale, const vector<vector<float>>& map) {
	vector<pair<Vector2, Vector2>> result;

	for(int y = 0; y < map.size() - 1; y++) {
		for(int x = 0; x < map[y].size() - 1; x++) {
			Vector2 pos(x, y);
			short int caseIndex = 0;

			vector<float> surfaceLevels(4);
			surfaceLevels[0] = map[y + 1][x];	 // Bottom left
			surfaceLevels[1] = map[y + 1][x + 1];// Bottom right
			surfaceLevels[2] = map[y][x + 1];	 // Top right
			surfaceLevels[3] = map[y][x];		 // Top left

			if(surfaceLevels[0] >= surfaceLevel) caseIndex |= 1; // Bottom left
			if(surfaceLevels[1] >= surfaceLevel) caseIndex |= 2; // Bottom right
			if(surfaceLevels[2] >= surfaceLevel) caseIndex |= 4; // Top right
			if(surfaceLevels[3] >= surfaceLevel) caseIndex |= 8; // Top left


			auto& edges = caseIndexToEdges[caseIndex];
			for(auto edgeCode : edges) {
				auto edgePair = CalculateEdge(surfaceLevel, interpolationAmount, edgeCode);
				edgePair.first = (edgePair.first + pos) * mapScale;
				edgePair.second = (edgePair.second + pos) * mapScale;

				result.push_back(edgePair);
			}
		}
	}

	return result;
}

void MapGenerator::GenerateMap(const float surfaceLevel, const float interpolationAmount, const vector<vector<float>>& map) {
	float mapScale = 500;

	// vector<pair<Vector2, Vector2>> lines = GenerateLineList(surfaceLevel, interpolationAmount, mapScale, map);

	Vector2 screenSize = {1920, 1080};
	SetTargetFPS(144);
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenSize.x, screenSize.y, "Shader test");
	// ToggleBorderlessWindowed();

	// Convert map to texture to feed into shader
	int mapHeight = map.size(), mapWidth = map[0].size();
	vector<unsigned char> flattenedMap(mapWidth * mapHeight);
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			flattenedMap[y * mapWidth + x] = (unsigned char)(map[y][x] * 255.0f);  // Convert float 0-1 to byte 0-255
		}
	}
	Image mapImage = {
		.data = flattenedMap.data(),         // Pointer to your image/map data
		.width = mapWidth,
		.height = mapHeight,
		.mipmaps = 1,           // Only one mipmap level
		.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE // Or whatever format you're using
	};

	Texture2D mapTexture = LoadTextureFromImage(mapImage);
	UnloadImage(mapImage);

	RenderTexture2D renderTexture = LoadRenderTexture(screenSize.x, screenSize.y);
	Shader postProcessingShader = LoadShader(0, TextFormat("../Shaders/squareMarchingPostProcessingShader.frag", 430));
	int ppScreenSizeLoc = GetShaderLocation(postProcessingShader, "screenSize");
	SetShaderValue(postProcessingShader, ppScreenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);

	Shader terrainShader = LoadShader(0, TextFormat("../Shaders/squareMarchingShader.frag", 430));

	int mapLoc = GetShaderLocation(terrainShader, "mapTexture");
	int screenSizeLoc = GetShaderLocation(terrainShader, "screenSize");
	int positionLoc = GetShaderLocation(terrainShader, "position");
	int surfaceLevelLoc = GetShaderLocation(terrainShader, "surfaceLevel");
	int terrainScaleLoc = GetShaderLocation(terrainShader, "terrainScale");
	int interpolationAmountLoc = GetShaderLocation(terrainShader, "interpolationAmount");
	SetShaderValue(terrainShader, screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(terrainShader, surfaceLevelLoc, &surfaceLevel, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, terrainScaleLoc, &mapScale, SHADER_UNIFORM_FLOAT);
	SetShaderValue(terrainShader, interpolationAmountLoc, &interpolationAmount, SHADER_UNIFORM_FLOAT);

	Vector2 pos = {0, 0};

	while(!WindowShouldClose()) {
		BeginTextureMode(renderTexture);
			ClearBackground(BLACK);

			Vector2 input(0, 0);
			if(IsKeyDown(KEY_W)) input.y -= 1.0;
			if(IsKeyDown(KEY_A)) input.x -= 1.0;
			if(IsKeyDown(KEY_S)) input.y += 1.0;
			if(IsKeyDown(KEY_D)) input.x += 1.0;
			input = V2Normalized(input);
			pos += input * GetFrameTime() * 0.9;
			SetShaderValue(terrainShader, positionLoc, &pos, SHADER_UNIFORM_VEC2);

			BeginShaderMode(terrainShader);
				SetShaderValueTexture(terrainShader, mapLoc, mapTexture);
				DrawRectangle(0, 0, screenSize.x, screenSize.y, WHITE);
			EndShaderMode();
		EndTextureMode();

		BeginDrawing();
			ClearBackground(MAGENTA);
			BeginShaderMode(postProcessingShader);
				DrawTextureRec(renderTexture.texture,
					{0, 0, screenSize.x, screenSize.y},
					{0, 0},
					WHITE);
			EndShaderMode();
		EndDrawing();

		// BeginDrawing();
		// ClearBackground(BLACK);
		//
		// Vector2 input(0, 0);
		// if(IsKeyDown(KEY_W)) input.y += 1.0;
		// if(IsKeyDown(KEY_A)) input.x -= 1.0;
		// if(IsKeyDown(KEY_S)) input.y -= 1.0;
		// if(IsKeyDown(KEY_D)) input.x += 1.0;
		// input = V2Normalized(input);
		// pos += input * GetFrameTime() * 0.9;
		// SetShaderValue(terrainShader, positionLoc, &pos, SHADER_UNIFORM_VEC2);
		//
		// BeginShaderMode(terrainShader);
		// SetShaderValueTexture(terrainShader, mapLoc, mapTexture);
		// DrawRectangle(0, 0, screenSize.x, screenSize.y, WHITE);
		// EndShaderMode();
		//
		// // for(auto line	 : lines) {
		// // 	DrawLineEx(line.first, line.second, 3.1, GREEN);
		// // }
		//
		// EndDrawing();
	}
	UnloadShader(terrainShader);
	CloseWindow();
}

