#version 430


in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D mapTexture;
uniform sampler2D oreAtlas;
uniform sampler2D oreColors;
uniform vec2 screenSize;
uniform vec2 position;
uniform float terrainScale;
uniform float surfaceLevel;
uniform float interpolationAmount;
uniform float chunkSize;
uniform vec2 chunkWorldSize;

uniform float time;

const int squareMarchingTable[16][4] = {
{-1, -1, -1, -1},// Case 0 // 0 Bottom
{0, 3, -1, -1},  // Case 1 // 1 Right
{1, 0, -1, -1},  // Case 2 // 2 Top
{1, 3, -1, -1},  // Case 3 // 3 Left
{2, 1, -1, -1},  // Case 4
{2, 3, 0, 1},    // Case 5
{2, 0, -1, -1},  // Case 6
{2, 3, -1, -1},  // Case 7
{3, 2, -1, -1},  // Case 8
{0, 2, -1, -1},  // Case 9
{1, 2, 3, 0},    // Case 10
{1, 2, -1, -1},  // Case 11
{3, 1, -1, -1},  // Case 12
{0, 1, -1, -1},  // Case 13
{3, 0, -1, -1},  // Case 14
{-1, -1, -1, -1},// Case 15
};

vec2 calculateEdgePoint(vec3 v1, vec3 v2){
    float t2 = 0.5;
    float t1 = (surfaceLevel - v1.z) / (v2.z - v1.z);
    float t = t1 + (t2 - t1) * interpolationAmount;
    return v1.xy + (v2.xy - v1.xy) * t;
}

float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

uint hash(uint state) {
    state ^= 2747636419u;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    return state;
}

float sampleOreShape(vec2 coord, vec2 texCoord, float randX, float randY){
    vec2 shapeAtlasCoord = mod(texCoords, 1.0) + vec2(randX, randY) * 0.1 + vec2(round(texture(mapTexture, coord, 0).b * 10.0) * 2, 0.0);
    shapeAtlasCoord -= vec2(1.0, 0.0);
    shapeAtlasCoord.x /= 7.0;
    return texture(oreAtlas, shapeAtlasCoord, 0).r;
}
vec3 sampleOreColor(vec2 coord){
    vec2 colorAtlasCoord = vec2(0.5, 0.5) + vec2(round(texture(mapTexture, coord, 0).b * 10.0), 0.0);
    colorAtlasCoord.x /= 4.0;
    return texture(oreColors, colorAtlasCoord, 0).rgb;
}

float rand01(uint state) {
    return hash(state) / 4294967295.0;
}

void main() {
    // Screen 0-1920/1080 coordinates
    vec2 coord =  vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 correctedPos = vec2(position.x - chunkWorldSize.x, screenSize.y - position.y - chunkWorldSize.y);
    coord = coord - correctedPos;
    coord /= chunkWorldSize;
    coord = mod(coord, 1.0); // Coordinates within the chunk 0-1

    vec2 texUnits = 1.0 / textureSize(mapTexture, 0);

    // Discard 1 pixel border
    coord *= chunkSize / (chunkSize + 2) + 0.0001;
    coord += vec2(texUnits.x, texUnits.y);

    vec2 texCoords = coord / texUnits;

    float randX = rand01(uint(ceil(texCoords.x) + ceil(texCoords.y) * chunkSize));
    float randY = rand01(uint(ceil(texCoords.x) + ceil(texCoords.y) * chunkSize) + 81736481);
    float randP = rand01(uint(ceil(gl_FragCoord.x - position.x)));
    randP = rand01(uint(randP * 1000) + uint(ceil(abs(-position.y - gl_FragCoord.y))));

    float oreVal = sampleOreShape(coord, texCoords, randX, randY);
    vec3 oreCol = sampleOreColor(coord);

    if(oreVal > 1.0 - (texture(mapTexture, coord, 0).g)){
        finalColor = vec4(oreCol, 1.0);
        return;
    }

    vec3 corners[4] = {
        vec3(floor(texCoords) + vec2(0, 1), texture(mapTexture, coord + vec2(0, texUnits.y), 0).r),  // Bottom left
        vec3(floor(texCoords) + vec2(1, 1), texture(mapTexture, coord + texUnits, 0).r),             // Bottom right
        vec3(floor(texCoords) + vec2(1, 0), texture(mapTexture, coord + vec2(texUnits.x, 0), 0).r),  // Top right
        vec3(floor(texCoords), texture(mapTexture, coord, 0).r)                                      // Top left
    };
    vec3 cornerColors[4] = {
        lerp(vec3(corners[0].z), sampleOreColor(coord + vec2(0, texUnits.y)), texture(mapTexture, coord, 0).g),
        lerp(vec3(corners[1].z), sampleOreColor(coord + texUnits), texture(mapTexture, coord, 0).g),
        lerp(vec3(corners[2].z), sampleOreColor(coord + vec2(texUnits.x, 0)), texture(mapTexture, coord, 0).g),
        lerp(vec3(corners[3].z), sampleOreColor(coord), texture(mapTexture, coord, 0).g)
    };

    // Bilinear filtering for the color
    vec2 bilinearT = mod(texCoords, 1.0);
    float avg = lerp(lerp(cornerColors[0].z, cornerColors[1].z, bilinearT.x), lerp(cornerColors[3].z, cornerColors[2].z, bilinearT.x), 1.0 - bilinearT.y);
    float bgAvg = avg * surfaceLevel / 2; // Added random noise to break up color banding
    bgAvg += randP * 0.005;
    // Remap avg from (0 to 1) range to (surfaceLevel to 1) range
    avg = max(avg, surfaceLevel);
    avg = (avg - surfaceLevel) / (1 - surfaceLevel);
    avg = 0.2 + (1.0 - 0.2) * avg;

    int caseIndex = 0;
    if(corners[0].z >= surfaceLevel) caseIndex |= 1;
    if(corners[1].z >= surfaceLevel) caseIndex |= 2;
    if(corners[2].z >= surfaceLevel) caseIndex |= 4;
    if(corners[3].z >= surfaceLevel) caseIndex |= 8;

    if(caseIndex == 0){
        finalColor = vec4(bgAvg, bgAvg, bgAvg,1.0);
        return;
    }

    if(caseIndex == 15){
        finalColor = vec4(avg, avg, avg,1.0);
        return;
    }

    vec2 edgePoints[4] = {
    calculateEdgePoint(corners[0], corners[1]), // 0 Bottom
    calculateEdgePoint(corners[1], corners[2]), // 1 Right
    calculateEdgePoint(corners[2], corners[3]), // 2 Top
    calculateEdgePoint(corners[3], corners[0]), // 3 Left
    };
    float dist = 1.0;
    int i = 0;
    while(i < 4 && squareMarchingTable[caseIndex][i] >= 0){
        vec2 p1 = edgePoints[squareMarchingTable[caseIndex][i]];
        vec2 p2 = edgePoints[squareMarchingTable[caseIndex][i + 1]];
        i += 2;

        // Line equation parameters: Ax + By + C = 0
        float A, B, C;
        A = p2.y - p1.y;
        B = p1.x - p2.x;
        C = p2.x * p1.y - p1.x * p2.y;
        float distToLine = A * texCoords.x + B * texCoords.y + C;

        dist = min(dist, distToLine);
    }

    if(dist > 0){
        finalColor = vec4(avg, avg, avg, 1.0);
    }else{
        finalColor = vec4(bgAvg, bgAvg, bgAvg, 1.0);
    }
}



/*
#version 430


in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D mapTexture;
uniform sampler2D oreAtlas;
uniform sampler2D oreColors;
uniform vec2 screenSize;
uniform vec2 position;
uniform float terrainScale;
uniform float surfaceLevel;
uniform float interpolationAmount;
uniform float chunkSize;
uniform vec2 chunkWorldSize;

uniform float time;

const int squareMarchingTable[16][4] = {
{-1, -1, -1, -1},// Case 0 // 0 Bottom
{0, 3, -1, -1},  // Case 1 // 1 Right
{1, 0, -1, -1},  // Case 2 // 2 Top
{1, 3, -1, -1},  // Case 3 // 3 Left
{2, 1, -1, -1},  // Case 4
{2, 3, 0, 1},    // Case 5
{2, 0, -1, -1},  // Case 6
{2, 3, -1, -1},  // Case 7
{3, 2, -1, -1},  // Case 8
{0, 2, -1, -1},  // Case 9
{1, 2, 3, 0},    // Case 10
{1, 2, -1, -1},  // Case 11
{3, 1, -1, -1},  // Case 12
{0, 1, -1, -1},  // Case 13
{3, 0, -1, -1},  // Case 14
{-1, -1, -1, -1},// Case 15
};

vec2 calculateEdgePoint(vec3 v1, vec3 v2){
    float t2 = 0.5;
    float t1 = (surfaceLevel - v1.z) / (v2.z - v1.z);
    float t = t1 + (t2 - t1) * interpolationAmount;
    return v1.xy + (v2.xy - v1.xy) * t;
}

float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

uint hash(uint state) {
    state ^= 2747636419u;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    return state;
}

float sampleOreShape(vec2 coord, vec2 texCoord, float randX, float randY){
    vec2 shapeAtlasCoord = mod(texCoords, 1.0) + vec2(randX, randY) * 0.1 + vec2(round(texture(mapTexture, coord, 0).b * 10.0) * 2, 0.0);
    shapeAtlasCoord -= vec2(1.0, 0.0);
    shapeAtlasCoord.x /= 7.0;
    return texture(oreAtlas, shapeAtlasCoord, 0).r;
}
vec3 sampleOreColor(vec2 coord){
    vec2 colorAtlasCoord = vec2(0.5, 0.5) + vec2(round(texture(mapTexture, coord, 0).b * 10.0), 0.0);
    colorAtlasCoord.x /= 4.0;
    return texture(oreColors, colorAtlasCoord, 0).rgb;
}

float rand01(uint state) {
    return hash(state) / 4294967295.0;
}

void main() {
    // Screen 0-1920/1080 coordinates
    vec2 coord =  vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 correctedPos = vec2(position.x - chunkWorldSize.x, screenSize.y - position.y - chunkWorldSize.y);
    coord = coord - correctedPos;
    coord /= chunkWorldSize;
    coord = mod(coord, 1.0); // Coordinates within the chunk 0-1

    vec2 texUnits = 1.0 / textureSize(mapTexture, 0);

    // Discard 1 pixel border
    coord *= chunkSize / (chunkSize + 2) + 0.0001;
    coord += vec2(texUnits.x, texUnits.y);

    vec2 texCoords = coord / texUnits;

    float randX = rand01(uint(ceil(texCoords.x) + ceil(texCoords.y) * chunkSize));
    float randY = rand01(uint(ceil(texCoords.x) + ceil(texCoords.y) * chunkSize) + 81736481);
    float randP = rand01(uint(ceil(gl_FragCoord.x - position.x)));
    randP = rand01(uint(randP * 1000) + uint(ceil(abs(-position.y - gl_FragCoord.y))));

    float oreVal = sampleOreShape(coord, texCoords, randX, randY);
    vec3 oreCol = sampleOreColor(coord);

    if(oreVal > 1.0 - (texture(mapTexture, coord, 0).g)){
        finalColor = vec4(oreCol, 1.0);
        return;
    }

    vec3 corners[4] = {
        vec3(floor(texCoords) + vec2(0, 1), texture(mapTexture, coord + vec2(0, texUnits.y), 0).r),  // Bottom left
        vec3(floor(texCoords) + vec2(1, 1), texture(mapTexture, coord + texUnits, 0).r),             // Bottom right
        vec3(floor(texCoords) + vec2(1, 0), texture(mapTexture, coord + vec2(texUnits.x, 0), 0).r),  // Top right
        vec3(floor(texCoords), texture(mapTexture, coord, 0).r)                                      // Top left
    };
    vec3 cornerColors[4] = {
        lerp(vec3(corners[0].z), sampleOreColor(coord + vec2(0, texUnits.y)), texture(mapTexture, coord, 0).g),
        lerp(vec3(corners[1].z), sampleOreColor(coord + texUnits), texture(mapTexture, coord, 0).g),
        lerp(vec3(corners[2].z), sampleOreColor(coord + vec2(texUnits.x, 0)), texture(mapTexture, coord, 0).g),
        lerp(vec3(corners[3].z), sampleOreColor(coord), texture(mapTexture, coord, 0).g)
    };

    // Bilinear filtering for the color
    vec2 bilinearT = mod(texCoords, 1.0);
    float avg = lerp(lerp(cornerColors[0].z, cornerColors[1].z, bilinearT.x), lerp(cornerColors[3].z, cornerColors[2].z, bilinearT.x), 1.0 - bilinearT.y);
    float bgAvg = avg * surfaceLevel / 2; // Added random noise to break up color banding
    bgAvg += randP * 0.005;
    // Remap avg from (0 to 1) range to (surfaceLevel to 1) range
    avg = max(avg, surfaceLevel);
    avg = (avg - surfaceLevel) / (1 - surfaceLevel);
    avg = 0.2 + (1.0 - 0.2) * avg;

    int caseIndex = 0;
    if(corners[0].z >= surfaceLevel) caseIndex |= 1;
    if(corners[1].z >= surfaceLevel) caseIndex |= 2;
    if(corners[2].z >= surfaceLevel) caseIndex |= 4;
    if(corners[3].z >= surfaceLevel) caseIndex |= 8;

    if(caseIndex == 0){
        finalColor = vec4(bgAvg, bgAvg, bgAvg,1.0);
        return;
    }

    if(caseIndex == 15){
        finalColor = vec4(avg, avg, avg,1.0);
        return;
    }

    vec2 edgePoints[4] = {
    calculateEdgePoint(corners[0], corners[1]), // 0 Bottom
    calculateEdgePoint(corners[1], corners[2]), // 1 Right
    calculateEdgePoint(corners[2], corners[3]), // 2 Top
    calculateEdgePoint(corners[3], corners[0]), // 3 Left
    };
    float dist = 1.0;
    int i = 0;
    while(i < 4 && squareMarchingTable[caseIndex][i] >= 0){
        vec2 p1 = edgePoints[squareMarchingTable[caseIndex][i]];
        vec2 p2 = edgePoints[squareMarchingTable[caseIndex][i + 1]];
        i += 2;

        // Line equation parameters: Ax + By + C = 0
        float A, B, C;
        A = p2.y - p1.y;
        B = p1.x - p2.x;
        C = p2.x * p1.y - p1.x * p2.y;
        float distToLine = A * texCoords.x + B * texCoords.y + C;

        dist = min(dist, distToLine);
    }

    if(dist > 0){
        finalColor = vec4(avg, avg, avg, 1.0);
    }else{
        finalColor = vec4(bgAvg, bgAvg, bgAvg, 1.0);
    }
}
*/