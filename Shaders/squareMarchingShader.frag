#version 430


in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D mapTexture;
uniform vec2 screenSize;
uniform vec2 position;
uniform float terrainScale;
uniform float surfaceLevel;
uniform float interpolationAmount;
uniform float chunkSize;
uniform vec2 chunkWorldSize;

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

void main() {
    // Screen 0-1920/1080 coordinates
    vec2 coord =  vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 correctedPos = vec2(position.x - chunkWorldSize.x, screenSize.y - position.y - chunkWorldSize.y);
    coord = coord - correctedPos;
    coord /= chunkWorldSize;
    coord = mod(coord, 1.0); // Coordinates within the chunk 0-1
    //    coord = coord * 1.5 - vec2(0.25); // Centering coordinates within chunk for debug

    // if(coord.x < 0 || coord.x > 1 || coord.y < 0 || coord.y > 1){
    //     finalColor = vec4(0,0,0,1);
    //     return;
    // }

    vec2 texUnits = 1.0 / textureSize(mapTexture, 0);

    // Discard 1 pixel border
    coord *= chunkSize / (chunkSize + 2) + 0.0001;
    coord += vec2(texUnits.x, texUnits.y);

    //    float tes = texture(mapTexture, coord, 0).r;
    //    finalColor = vec4(tes, tes, tes, 1.0);
    //
    //    return;

    vec2 texCoords = coord / texUnits;

    vec3 corners[4] = {
        vec3(floor(texCoords) + vec2(0, 1), texture(mapTexture, coord + vec2(0, texUnits.y), 0).r),  // Bottom left
        vec3(floor(texCoords) + vec2(1, 1), texture(mapTexture, coord + texUnits, 0).r),             // Bottom right
        vec3(floor(texCoords) + vec2(1, 0), texture(mapTexture, coord + vec2(texUnits.x, 0), 0).r),  // Top right
        vec3(floor(texCoords), texture(mapTexture, coord, 0).r)                                      // Top left
    };

    // Bilinear filtering for the color
    vec2 bilinearT = mod(texCoords, 1.0);
    float avg = lerp(lerp(corners[0].z, corners[1].z, bilinearT.x), lerp(corners[3].z, corners[2].z, bilinearT.x), 1.0 - bilinearT.y);
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
        finalColor = vec4(0.0,0.0,0.0,1.0);
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

    if(dist > 0) dist = 1.0;

    finalColor = vec4(dist * avg, dist * avg, dist * avg, 1.0);
}
