#version 430


in vec2 fragTexCoord;
out vec4 finalColor;

uniform vec2 meterSize;

uniform float time;
uniform float fill;

float sdRoundBox(in vec2 p, in vec2 b, in vec4 r) {
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

float pingPong(float t, float a, float b){
    return mix(a, b, abs(fract(t) * 2.0 - 1.0));
}

void main() {
    // UV 0-1 coordinates
    vec2 uv = fragTexCoord * meterSize;

    float dist = -sdRoundBox(uv - meterSize / 2, meterSize / 2, vec4(10, meterSize.x / 2, 10, meterSize.x / 2));
    if(dist <= 0) discard;

    dist *= 0.1;
    if(dist < 1) finalColor = vec4(1, 0, 0, 1);
    else finalColor = vec4(fract(dist), fract(dist), fract(dist), 1);
}
