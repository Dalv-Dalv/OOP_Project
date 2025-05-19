#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform int index;
uniform float time;
uniform float highlight;
uniform float roundness;
uniform sampler2D icon;

// https://www.shadertoy.com/view/fs3yDM
vec3 sdSquircle(vec2 p, float n) {
    // symmetries
    vec2 k = sign(p); p = abs(p);
    bool m = p.y>p.x; if( m ) p=p.yx;

    const int num = 16; // tesselate into 8x16=128 segments, more denselly at the corners
    float s = 1.0;
    float d = 1e20;
    vec2 oq = vec2(1.0,0.0);
    vec2  g = vec2(0.0,0.0);
    for( int i=1; i<=num; i++ ) {
        float h = (6.283185/8.0)*float(i)/float(num);
        vec2  q = pow(vec2(cos(h),sin(h)),vec2(2.0/n));
        vec2  pa = p-oq;
        vec2  ba = q-oq;
        vec2  z = pa - ba*clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
        float d2 = dot(z,z);
        if( d2<d )
        {
            d = d2;
            s = pa.x*ba.y - pa.y*ba.x;
            g = z;
        }
        oq = q;
    }

    // undo symmetries
    if( m ) g=g.yx; g*=k;

    d = sign(s)*sqrt(d);
    return vec3( d, g/d );
}

float sdCircle(vec2 p, float r){
    return length(p) - r;
}

float pingPong(float t, float a, float b){
    return mix(a, b, abs(fract(t) * 2.0 - 1.0));
}

void main() {
    // UV 0-1 coordinates
    vec2 uv = fragTexCoord;

    float dist = sdSquircle(uv * 2.8 - vec2(1.4), roundness + highlight).r;
    float distCircle = sdCircle(uv - vec2(0.5), 0.5);


    float alpha;
    if(dist < 0) alpha = 1 - abs(distCircle) * 0.1;
    else alpha = 1 - abs(dist);

    alpha = clamp(alpha, 0, 1);
    alpha = alpha * alpha * alpha;
    alpha = 1 - ((alpha - 1.0) / (0.6 - 1.0));

    vec3 col = vec3(1);
    if(dist < 0){
        col = vec3(0.5);
    }

    col = vec3(col.r, col.gb * (1 - highlight));

    vec4 texCol = texture(icon, uv * 2 - vec2(0.5), 0).rgba;
    finalColor = mix(vec4(col, alpha), vec4(texCol.rgb, 1), texCol.a);
}
