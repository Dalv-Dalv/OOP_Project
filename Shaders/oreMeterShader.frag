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
    vec2 uv = fragTexCoord;
    uv.y = 1 - uv.y;
    uv *= meterSize;

    float dist = -sdRoundBox(uv - meterSize / 2, meterSize / 2, vec4(meterSize.x / 2, 5, meterSize.x / 2, 5));
    float alpha = clamp(smoothstep(0, 1, dist), 0, 1);

    dist /= meterSize.x / 2;

    vec3 col = vec3(237, 242, 244) / 255;

    float fill = pingPong(time, 0, 1);

    if(dist < 0.2){
        finalColor = vec4(col, alpha);
    }else{
        if(uv.y - sin(time + uv.x * 0.02) * 4 < meterSize.y * fill){
            float gr = (uv.y - sin(time + uv.x * 0.02) * 4) / (meterSize.y * fill);
            finalColor = vec4(mix(vec3(0.8, 0.1, 0.3), vec3(1,0,0), gr), alpha);
        }else if(uv.y - cos(-time + uv.x * 0.02) * 4 < meterSize.y * fill){
            finalColor = vec4(0.6, 0, 0, alpha);
        }else{
            finalColor = mix(vec4(col, 0.4), vec4(col * 0.8, 0.3), dist);
        }
    }
}
