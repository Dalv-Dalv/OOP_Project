#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 screenSize;
uniform vec2 position;
uniform float time;
uniform float testParam;

void main() {
    vec2 coord =  vec2(gl_FragCoord.x, gl_FragCoord.y) / screenSize.y + position;

    finalColor = vec4(testParam, 0.0 , 0.0, 1.0);
}
