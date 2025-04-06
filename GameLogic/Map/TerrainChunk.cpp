#include "TerrainChunk.h"

#include <iostream>
#include <external/glad.h>

#include <raymath.h>

#include "../../config.h"
#include "../../cmake-build-release/_deps/raylib-src/src/external/tinyobj_loader_c.h"
#include "../../CoreGameLogic/GameCamera.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../../Utilities/GameUtilities.h"

const vector<vector<int>> TerrainChunk::squareMarchingTable = {
	{},			 // Case 0 // 0 Bottom
	{0, 3},		 // Case 1 // 1 Right
	{1, 0},		 // Case 2 // 2 Top
	{1, 3},		 // Case 3 // 3 Left
	{2, 1},		 // Case 4
	{2, 3, 0, 1},// Case 5
	{2, 0},  	 // Case 6
	{2, 3},  	 // Case 7
	{3, 2},  	 // Case 8
	{0, 2},  	 // Case 9
	{1, 2, 3, 0},// Case 10
	{1, 2},  	 // Case 11
	{3, 1},  	 // Case 12
	{0, 1},  	 // Case 13
	{3, 0},  	 // Case 14
	{},		     // Case 15
};

void TerrainChunk::UpdateGPUData() {
	auto& imageData = cpuData->GetFlattenedValues();
	UpdateTexture(gpuData, imageData.data());
}

float TerrainChunk::MiningFalloff(float radius, float distSqr) {
	if(distSqr > radius * radius || distSqr < 0) return 0;

	distSqr = sqrt(distSqr);

	// linear
	float t = GameUtilities::inverseLerp(0, radius, distSqr);

	// quadratic
	t = 1 - t * t * t;
	return t;
}

Vector2 TerrainChunk::CalculateEdgePoint(Vector2 v1, float w1, Vector2 v2, float w2, float surfaceLevel) const {
	float t2 = 0.5;
	float t1 = (surfaceLevel - w1) / (w2 - w1);
	float t = t1 + (t2 - t1) * TERRAIN_INTERPOLATION_AMOUNT;
	return v1 + (v2 - v1) * t;
}

void TerrainChunk::CheckMinCollisionAt(Vector2 pos, int posx, int posy, CollisionInfo& prevMin, float surfaceLevel, float unit) const  {
	float offsX = position.x + (posx - 1) * unit;
	float offsY = position.y + (chunkHeight - posy - 1) * unit;

	Vector2 corners[4] ={
		Vector2(offsX       , offsY - unit),
		Vector2(offsX + unit, offsY - unit),
		Vector2(offsX + unit, offsY       ),
		Vector2(offsX       , offsY       )
	};

	float cornerWeights[4] = {
		cpuData->GetValueAt(posx    , posy + 1) / 255.0f,
		cpuData->GetValueAt(posx + 1, posy + 1) / 255.0f,
		cpuData->GetValueAt(posx + 1, posy    ) / 255.0f,
		cpuData->GetValueAt(posx    , posy    ) / 255.0f
	};

	int caseIndex = 0;
	if(cornerWeights[0] >= surfaceLevel) caseIndex |= 1;
	if(cornerWeights[1] >= surfaceLevel) caseIndex |= 2;
	if(cornerWeights[2] >= surfaceLevel) caseIndex |= 4;
	if(cornerWeights[3] >= surfaceLevel) caseIndex |= 8;
	if(caseIndex == 0) return;
	if(caseIndex == 15) {
		prevMin = min(prevMin, {100000, {0, 0}, pos});
		return;
	}

	Vector2 edgePoints[4] = {
		CalculateEdgePoint(corners[0], cornerWeights[0], corners[1], cornerWeights[1], surfaceLevel),
		CalculateEdgePoint(corners[1], cornerWeights[1], corners[2], cornerWeights[2], surfaceLevel),
		CalculateEdgePoint(corners[2], cornerWeights[2], corners[3], cornerWeights[3], surfaceLevel),
		CalculateEdgePoint(corners[3], cornerWeights[3], corners[0], cornerWeights[0], surfaceLevel)
	};

	for(int i = 0; i < squareMarchingTable[caseIndex].size(); i += 2) {
		const Vector2& p1 = edgePoints[squareMarchingTable[caseIndex][i]];
		const Vector2& p2 = edgePoints[squareMarchingTable[caseIndex][i + 1]];

		DrawLineEx(p1, p2, 4, RED);

		float dist = 0;
		const Vector2 seg = p2 - p1;
		const Vector2 toPoint = pos - p1;
		Vector2 normal = {0, 0};
		Vector2 projection = pos;

		const float lenSqr = seg.x * seg.x + seg.y * seg.y;
		if(lenSqr == 0.0) dist = 0;
		else {
			const float t = max(0.0f, min(1.0f, Vector2DotProduct(toPoint, seg) / lenSqr));
			projection = p1 + seg * t;

			normal = pos - projection;

			DrawLineEx(pos, projection, 2, Color(0, 255, 0, 100));

			dist = Vector2Distance(pos, projection);
		}

		// Signed distance
		float cross = seg.x * toPoint.y - seg.y * toPoint.x;
		dist *= (cross <= 0 ? 1 : -1);

		prevMin = min(prevMin, {dist, normal, projection});
	}
}

TerrainChunk::TerrainChunk(Vector2 position, int width, int height, int chunkWidth, int chunkHeight, TerrainData* mapData)
: position(position), width(width), height(height), chunkWidth(chunkWidth), chunkHeight(chunkHeight), cpuData(mapData){
	auto& imageData = mapData->GetFlattenedValues();

	Image mapImage {
		.data = imageData.data(),
		.width = mapData->GetWidth(),
		.height = mapData->GetHeight(),
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE
	};

	gpuData = LoadTextureFromImage(mapImage);
}
TerrainChunk::~TerrainChunk() {
	delete cpuData;
}


void TerrainChunk::Render(Shader& shader, int textureLoc, int posLoc) {
	Vector2 newPos(position);
	auto camPos = GameCamera::GetActiveCamera()->GetGameObject()->position * GameCamera::GetActiveCamera()->GetZoom();
	auto offset = GameCamera::GetActiveCamera()->GetOffsetPos();
	newPos.x -= camPos.x - offset.x;
	newPos.y -= camPos.y - offset.y;
	BeginShaderMode(shader);
		SetShaderValueTexture(shader, textureLoc, gpuData);
		SetShaderValue(shader, posLoc, &newPos, SHADER_UNIFORM_VEC2);
		DrawRectangle(newPos.x, newPos.y, width + 1, height + 1, WHITE);
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
			else val -= miningPower * deltaTime * MiningFalloff(radius, distSqr);
			cpuData->SetValueAt(x, y, val);
		}
	}

	UpdateGPUData();
}

CollisionInfo TerrainChunk::CheckCollisions(Vector2 pos, int posx, int posy, float radius, float surfaceLevel, float unit) {
	CollisionInfo res(1000000000, {0, 0}, pos);
	Highlight();
	for(int x = posx - radius; x < posx + radius; x++) {
		if(x < 0) continue;
		if(x > chunkWidth - 2) break;

		for(int y = posy - radius; y < posy + radius; y++) {
			if(y < 0) continue;
			if(y > chunkHeight - 2) break;

			CheckMinCollisionAt(pos, x, y, res, surfaceLevel, unit);
		}
	}
	return res;
}

void TerrainChunk::Highlight() {
	DrawRectangle(position.x, position.y, width + 1, height + 1, Color(100, 100, 255, 40));
}


// Getters
const Vector2& TerrainChunk::GetPosition() const { return position; }