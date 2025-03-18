#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;

void main() {
    vec4 color = texture(texture0, fragTexCoord);
    vec4 total = vec4(0);
    float magnitude = sin(time) * 0.1 + cos(time * 1.391847) * 0.2;
    vec2 centeredCoords = fragTexCoord  * 2.0 - vec2(1.0, 1.0);
    total += texture(texture0, fragTexCoord + vec2(1, 0) * magnitude + centeredCoords * centeredCoords * centeredCoords * 5.0 * magnitude);
    total += texture(texture0, fragTexCoord + vec2(-1, 0) * magnitude + centeredCoords * centeredCoords * centeredCoords * 5.0 * magnitude);
    total += texture(texture0, fragTexCoord + vec2(0, 1) * magnitude + centeredCoords * centeredCoords * centeredCoords * 5.0 * magnitude);
    total += texture(texture0, fragTexCoord + vec2(0, -1) * magnitude + centeredCoords * centeredCoords * centeredCoords * 5.0 * magnitude);

    total += texture(texture0, fragTexCoord + vec2(1, 1) * magnitude + centeredCoords * centeredCoords * centeredCoords * magnitude);
    total += texture(texture0, fragTexCoord + vec2(1, -1) * magnitude + centeredCoords * centeredCoords * centeredCoords * magnitude);
    total += texture(texture0, fragTexCoord + vec2(-1, 1) * magnitude + centeredCoords * centeredCoords * centeredCoords * magnitude);
    total += texture(texture0, fragTexCoord + vec2(-1, -1) * magnitude + centeredCoords * centeredCoords * centeredCoords * magnitude);

    total /= 8;
    finalColor = vec4(total.rgb, 1.0);
}
