#include "TerrainChunk.h"

#include <iostream>
#include <optional>
#include <external/glad.h>

#include <raymath.h>

#include "Terrain.h"
#include "../../config.h"
#include "../../cmake-build-release/_deps/raylib-src/src/rlgl.h"
#include "../../CoreGameLogic/AssetManager.h"
#include "../../CoreGameLogic/GameCamera.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../../Utilities/GameUtilities.h"
#include "../../Utilities/Vector2Utils.h"

Sound TerrainChunk::orePickupSound;

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
	auto& imageData = cpuData->GetTerrainValues();
	UpdateTexture(gpuData, imageData.data());
}

TerrainChunk::TerrainChunk(Vector2 position, int width, int height, int chunkWidth, int chunkHeight, TerrainData* mapData)
: position(position), width(width), height(height), chunkWidth(chunkWidth), chunkHeight(chunkHeight), cpuData(mapData){
	auto& imageData = mapData->GetTerrainValues();

	Image mapImage {
		.data = imageData.data(),
		.width = mapData->GetWidth(),
		.height = mapData->GetHeight(),
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
	};

	orePickupSound = AssetManager::LoadSound("Sounds/Ore_Pickup.wav");

	gpuData = LoadTextureFromImage(mapImage);
	SetTextureFilter(gpuData, TEXTURE_FILTER_POINT);
}
TerrainChunk::~TerrainChunk() {
	delete cpuData;
}


void TerrainChunk::Render(Shader& shader, int textureLoc, int posLoc, int atlasLoc, const Texture2D& oreAtlas, int oreColorsLoc, const Texture2D& oreColors, int mapColorsLoc, const Texture2D& mapColors) {
	Vector2 newPos(position);
	auto camPos = GameCamera::GetActiveCamera()->GetGameObject()->position * GameCamera::GetActiveCamera()->GetZoom();
	auto offset = GameCamera::GetActiveCamera()->GetOffsetPos();
	newPos.x -= camPos.x - offset.x;
	newPos.y -= camPos.y - offset.y;

	float time = GetTime();

	BeginShaderMode(shader);
		SetShaderValueTexture(shader, textureLoc, gpuData);
		SetShaderValueTexture(shader, atlasLoc, oreAtlas);
		SetShaderValueTexture(shader, oreColorsLoc, oreColors);
		SetShaderValueTexture(shader, mapColorsLoc, mapColors);
		SetShaderValue(shader, GetShaderLocation(shader, "time"), &time, SHADER_UNIFORM_FLOAT);
		SetShaderValue(shader, posLoc, &newPos, SHADER_UNIFORM_VEC2);
		DrawRectangle(newPos.x, newPos.y, width + 1, height + 1, WHITE);
	EndShaderMode();
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
void TerrainChunk::MineAt(int posx, int posy, float radius, float miningPower, float deltaTime, OreInfo& miningResult) {
	float surfaceLevel = Terrain::GetActiveTerrain()->GetSurfaceLevel();
	posy -= 1;

	for(int x = posx - radius; x < posx + radius; x++) {
		if(x < 0) continue;
		if(x > chunkWidth) break;

		for(int y = posy - radius; y < posy + radius; y++) {
			if(y < 0) continue;
			if(y > chunkHeight) break;

			// Distance squared
			float distSqr = (posx - x) * (posx - x) + (posy - y) * (posy - y);
			float falloff = MiningFalloff(radius, distSqr);

			auto val = cpuData->GetValueAt(x, y);

			if(val.surfaceValue < 0.1) continue;

			if(val.oreValue <= 0 || val.oreType < 0.05) {
				if(val.hardness < 0.05) continue;
				val.surfaceValue -= Clamp(miningPower * deltaTime * falloff * val.hardness, 0, 1);
			} else {
				float oreMined = Clamp(miningPower * deltaTime * falloff, 0, val.oreValue);
				val.oreValue -= oreMined;

				if(val.oreValue <= 0) {
					miningResult.AddOreFromMap(val.oreType, 1);
					PlaySound(orePickupSound);
				}
			}

			cpuData->SetValueAt(x, y, val);
		}
	}

	UpdateGPUData();
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
		cpuData->GetValueAt(posx    , posy + 1).surfaceValue,
		cpuData->GetValueAt(posx + 1, posy + 1).surfaceValue,
		cpuData->GetValueAt(posx + 1, posy    ).surfaceValue,
		cpuData->GetValueAt(posx    , posy    ).surfaceValue
	};

	int caseIndex = 0;
	if(cornerWeights[0] >= surfaceLevel) caseIndex |= 1;
	if(cornerWeights[1] >= surfaceLevel) caseIndex |= 2;
	if(cornerWeights[2] >= surfaceLevel) caseIndex |= 4;
	if(cornerWeights[3] >= surfaceLevel) caseIndex |= 8;
	if(caseIndex == 0) return;
	if(caseIndex == 15) {
		prevMin = min(prevMin, {-100000, {0, 0}, pos});
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

		// DrawLineEx(p1, p2, 4, RED);

		float dist = 0;
		const Vector2 seg = p2 - p1;
		const Vector2 toPoint = pos - p1;
		const float lenSqr = seg.x * seg.x + seg.y * seg.y;

		Vector2 normal = { -seg.y, seg.x};
		normal *= GameUtilities::fastInverseSqrt(lenSqr);

		Vector2 projection = pos;
		if(lenSqr == 0.0) dist = 0;
		else {
			const float t = max(0.0f, min(1.0f, Vector2DotProduct(toPoint, seg) / lenSqr));
			projection = p1 + seg * t;

			// DrawLineEx(pos, projection, 2, Color(0, 255, 0, 100));

			dist = Vector2Distance(pos, projection);
		}

		// Signed distance
		float cross = seg.x * toPoint.y - seg.y * toPoint.x;
		dist *= (cross <= 0 ? 1 : -1);

		prevMin = min(prevMin, {dist, normal, projection});
	}
}

CollisionInfo TerrainChunk::CheckCollisions(Vector2 pos, int posx, int posy, float radius, float surfaceLevel, float unit) {
	CollisionInfo res(1000000000, {0, 0}, pos);
	// Highlight();
	for(int x = posx - radius + 1; x < posx + radius + 1; x++) {
		if(x < 1) continue;
		if(x > chunkWidth - 2) break;

		for(int y = posy - radius - 1; y < posy + radius - 1; y++) {
			if(y < 1) continue;
			if(y > chunkHeight - 2) break;

			CheckMinCollisionAt(pos, x, y, res, surfaceLevel, unit);
		}
	}
	return res;
}

void TerrainChunk::RaySegmentIntersection(Vector2 rOrigin, Vector2 rDir, Vector2 p1, Vector2 p2, RaycastHitInfo& hitInfo) {
	Vector2 seg = p2 - p1;
	float denominator = seg.x * rDir.y - seg.y * rDir.x;
	// Lines are close to parallel
	if(abs(denominator) < 0.000001) { return; }

	Vector2 diff = rOrigin - p1;
	float t = (diff.x * rDir.y - diff.y * rDir.x) / denominator;
	float u = (diff.x * seg.y - diff.y * seg.x) / denominator;

	if(t >= 0.0f && t <= 1.0f && u >= 0.0f) {
		RaycastHitInfo minRay = hitInfo;
		minRay.normal = { seg.y, -seg.x};
		minRay.normal *= GameUtilities::fastInverseSqrt(seg.x * seg.x + seg.y * seg.y);
		minRay.hitPoint = p1 + seg * t;
		minRay.hitDistance = GameUtilities::Distance(hitInfo.origin, minRay.hitPoint);
		minRay.hasHit = true;
		hitInfo = min(minRay, hitInfo);
	}
}

void TerrainChunk::CheckRay(Vector2 entryPos, Vector2 dir, int cellX, int cellY, float unit, float surfaceLevel, RaycastHitInfo& hitInfo) {
	cellX = cellX + 1;
	cellY = chunkHeight - cellY - 2;
	if(cellX < 1 || cellX > chunkWidth - 2) return;
	if(cellY < 1 || cellY > chunkHeight - 2) return;

	float offsX = position.x + (cellX - 1) * unit;
	float offsY = position.y + (chunkHeight - cellY - 1) * unit;

	Vector2 corners[4] ={
		Vector2(offsX       , offsY - unit),
		Vector2(offsX + unit, offsY - unit),
		Vector2(offsX + unit, offsY       ),
		Vector2(offsX       , offsY       )
	};

	float cornerWeights[4] = {
		cpuData->GetValueAt(cellX    , cellY + 1).surfaceValue,
		cpuData->GetValueAt(cellX + 1, cellY + 1).surfaceValue,
		cpuData->GetValueAt(cellX + 1, cellY    ).surfaceValue,
		cpuData->GetValueAt(cellX    , cellY    ).surfaceValue
	};

	int caseIndex = 0;
	if(cornerWeights[0] >= surfaceLevel) caseIndex |= 1;
	if(cornerWeights[1] >= surfaceLevel) caseIndex |= 2;
	if(cornerWeights[2] >= surfaceLevel) caseIndex |= 4;
	if(cornerWeights[3] >= surfaceLevel) caseIndex |= 8;
	if(caseIndex == 0) return;

	if(caseIndex == 15) {
		hitInfo.hitPoint = entryPos;
		hitInfo.hasHit = true;
		hitInfo.normal = {-dir.x, -dir.y};
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

		RaySegmentIntersection(entryPos, dir, p1, p2, hitInfo);
	}
}

/// @param dir - Normalized direction
/// @param hitInfo
void TerrainChunk::CheckRaycast(Vector2 origin, Vector2 dir, float maxDistance, float unit, float surfaceLevel, RaycastHitInfo& hitInfo) {
	// Amanatides-Woo fast voxel traversal algorithm
	Vector2 localOrigin = origin - position;
	// Highlight();


	int cellX = floor(localOrigin.x / unit);
	int cellY = floor(localOrigin.y / unit);

	int stepX, stepY;
	float mx, my;
	float dx, dy;

	if(dir.x > 0.0f) {
		stepX = 1;
		mx = ((cellX + 1) * unit - localOrigin.x) / dir.x;
		dx = unit / dir.x;
	} else if(dir.x < 0.0f) {
		stepX = -1;
		mx = (localOrigin.x - cellX * unit) / (-dir.x);
		dx = unit / (-dir.x);
	} else {
		stepX = 0;
		mx = numeric_limits<float>::infinity();
		dx = numeric_limits<float>::infinity();
	}

	if(dir.y > 0.0f) {
		stepY = 1;
		my = ((cellY + 1) * unit - localOrigin.y) / dir.y;
		dy = unit / dir.y;
	} else if(dir.y < 0.0f) {
		stepY = -1;
		my = (localOrigin.y - cellY * unit) / (-dir.y);
		dy = unit / (-dir.y);
	} else {
		stepY = 0;
		my = numeric_limits<float>::infinity();
		dy = numeric_limits<float>::infinity();
	}

	Vector2 entryPos = localOrigin;
	CheckRay(origin, dir, cellX, cellY, unit, surfaceLevel, hitInfo);

	float traveled = 0;
	while(traveled <= maxDistance) {
		if(mx < my) {
			traveled = mx;
			if(traveled > maxDistance) break;
			cellX += stepX;
			mx += dx;
		} else {
			traveled = my;
			if(traveled > maxDistance) break;
			cellY += stepY;
			my += dy;
		}
		entryPos = localOrigin + dir * traveled;

		CheckRay(origin, dir, cellX, cellY, unit, surfaceLevel, hitInfo);
		if(hitInfo.hasHit) return;
	}
}


void TerrainChunk::Highlight() {
	DrawRectangle(position.x, position.y, width + 1, height + 1, Color(100, 100, 255, 40));
}


// Getters
const Vector2& TerrainChunk::GetPosition() const { return position; }
