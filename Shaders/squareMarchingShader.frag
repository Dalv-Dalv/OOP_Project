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
float mix(float a, float b, float t){
    return lerp(a, b, t*t*(3.0-2.0*t)); // smoothstep
}

vec3 lerp(vec3 a, vec3 b, float t){
    return a + (b - a) * t;
}
vec3 mix(vec3 a, vec3 b, float t){
    return lerp(a, b, t*t*(3.0-2.0*t)); // smoothstep
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
float rand01(uint state) {
    return hash(state) / 4294967295.0;
}

float sampleOreShape(vec2 coord, vec2 texCoords){
    texCoords += vec2(0.5);
    float randX = rand01(uint(ceil(texCoords.x) + ceil(texCoords.y) * chunkSize));
    float randY = rand01(uint(ceil(texCoords.x) + ceil(texCoords.y) * chunkSize) + 81736481);

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

vec3 stylizeTerrainColor(float terrainVal, float hardness){
    vec3 col, hardCol;
    if(terrainVal > surfaceLevel){
        // Map [surfaceLevel, 1] to [0, 1]
        terrainVal = (terrainVal - surfaceLevel) / (1 - surfaceLevel);
        //        terrainVal = 0.2 + (1.0 - 0.2) * terrainVal;
        col = lerp(vec3(0.109803922, 0.364705882, 0.364705882), vec3(0.192156863, 0.301960784, 0.388235294) * 0.5, terrainVal);
        hardCol = lerp(vec3(0.209803922, 0.364705882, 0.764705882), vec3(0.109803922, 0.364705882, 0.564705882) * 0.5, terrainVal);
    }else{
        // Map [0, surfaceLevel] to [0, 1]
        terrainVal = terrainVal / surfaceLevel;
        col = lerp(vec3(0.0431372549, 0.11372549, 0.149019608) * 0.5, vec3(0.0745098039, 0.180392157, 0.211764706) * 2, terrainVal);
        hardCol = vec3(0.2431372549, 0.11372549, 0.749019608) * 0.2;
    }

    col = lerp(hardCol, col, hardness);
    return col;
}

void main() {
    // Screen 0-1920/1080 coordinates
    vec2 coord =  vec2(ceil(gl_FragCoord.x), floor(gl_FragCoord.y));
    vec2 correctedPos = vec2(position.x - chunkWorldSize.x, screenSize.y - position.y - chunkWorldSize.y);
    coord = coord - correctedPos;
    coord /= chunkWorldSize;
    coord.x = clamp(coord.x - 1, 0, 1); // Fix border artifacts
    coord.y = clamp(coord.y, 0, 1);

    vec2 texUnits = 1.0 / textureSize(mapTexture, 0);

    // Discard 1 pixel border
    coord *= chunkSize / (chunkSize + 2);
    coord += vec2(texUnits.x, texUnits.y);

    vec2 texCoords = coord / texUnits;

    float oreVal = sampleOreShape(coord + texUnits / 2, texCoords);
    vec3 oreCol = sampleOreColor(coord + texUnits / 2);

    if(oreVal > 1.0 - (texture(mapTexture, coord + texUnits / 2, 0).g)){
        finalColor = vec4(oreCol, 1.0);
        return;
    }

    vec4 corners[4] = {
    vec4(floor(texCoords) + vec2(0, 1), texture(mapTexture, coord + vec2(0, texUnits.y), 0).ra),  // Bottom left
    vec4(floor(texCoords) + vec2(1, 1), texture(mapTexture, coord + texUnits, 0).ra),             // Bottom right
    vec4(floor(texCoords) + vec2(1, 0), texture(mapTexture, coord + vec2(texUnits.x, 0), 0).ra),  // Top right
    vec4(floor(texCoords), texture(mapTexture, coord, 0).ra)                                      // Top left
    };
    vec3 cornerColors[4] = {
    lerp(vec3(stylizeTerrainColor(corners[0].z, corners[0].a)), sampleOreColor(coord + vec2(0, texUnits.y)), texture(mapTexture, coord + vec2(0, texUnits.y), 0).g),
    lerp(vec3(stylizeTerrainColor(corners[1].z, corners[1].a)), sampleOreColor(coord + texUnits),            texture(mapTexture, coord + texUnits, 0).g),
    lerp(vec3(stylizeTerrainColor(corners[2].z, corners[2].a)), sampleOreColor(coord + vec2(texUnits.x, 0)), texture(mapTexture, coord + vec2(texUnits.x, 0), 0).g),
    lerp(vec3(stylizeTerrainColor(corners[3].z, corners[3].a)), sampleOreColor(coord),                       texture(mapTexture, coord, 0).g)
    };

    // Bilinear filtering for the color
    // QOL: Use barycentric interpolation with 2 triangles instead of bilinear to avoid triangular-squarish aspect
    vec2 bilinearT = mod(texCoords, 1.0);
    vec3 avg = mix(mix(cornerColors[0], cornerColors[1], bilinearT.x), mix(cornerColors[3], cornerColors[2], bilinearT.x), 1.0 - bilinearT.y);

    // Adding random noise to break up color banding
    float randP = rand01(uint(ceil(gl_FragCoord.x - position.x)));
    randP = rand01(uint(randP * 1000) + uint(ceil(abs(-position.y - gl_FragCoord.y))));
    avg += vec3(randP) * 0.005;
    vec3 bgAvg = avg * surfaceLevel;

    int caseIndex = 0;
    if(corners[0].z >= surfaceLevel) caseIndex |= 1;
    if(corners[1].z >= surfaceLevel) caseIndex |= 2;
    if(corners[2].z >= surfaceLevel) caseIndex |= 4;
    if(corners[3].z >= surfaceLevel) caseIndex |= 8;

    if(caseIndex == 0){
        finalColor = vec4(bgAvg,1.0);
        return;
    }

    if(caseIndex == 15){
        finalColor = vec4(avg,1.0);
        return;
    }

    vec2 edgePoints[4] = {
    calculateEdgePoint(corners[0].rgb, corners[1].rgb), // 0 Bottom
    calculateEdgePoint(corners[1].rgb, corners[2].rgb), // 1 Right
    calculateEdgePoint(corners[2].rgb, corners[3].rgb), // 2 Top
    calculateEdgePoint(corners[3].rgb, corners[0].rgb), // 3 Left
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
        finalColor = vec4(avg, 1.0);
    }else{
        finalColor = vec4(bgAvg, 1.0);
    }
}
