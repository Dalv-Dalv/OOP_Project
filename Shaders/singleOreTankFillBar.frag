#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform float oreType;
uniform float time;
uniform float fillAmount;
uniform sampler2D oreColors;

// https://www.shadertoy.com/view/fs3yDM

float pingPong(float t, float a, float b){
    return mix(a, b, abs(fract(t) * 2.0 - 1.0));
}

void main() {
    // UV 0-1 coordinates
    vec2 uv = fragTexCoord;

    vec4 col = vec4(0.3);
    if(uv.x < fillAmount){
        float oreCoord = (oreType + 1) / 4;
        vec3 oreCol = texture(oreColors, vec2(oreCoord + 0.05, 0.5)).rgb;

        float highlightVal = abs(sin(time * (fillAmount * 10 + 1) + (uv.x + uv.y * 0.1) * 5));
        col = vec4(oreCol + vec3(highlightVal) * 0.2,1);
    }

    finalColor = col;
}
