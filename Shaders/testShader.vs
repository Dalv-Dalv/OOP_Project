#version 330

attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;

out vec2 fragTexCoord;

void main() {
	fragTexCoord = vertexTexCoord;
}