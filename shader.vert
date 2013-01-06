#version 420
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
 
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
 
out vec4 vpeye;
out vec4 vneye;
 
void main() {
	vpeye = modelViewMatrix * vec4(vertexPosition, 1.0);
	vneye = vec4(normalMatrix * vertexNormal, 0);
	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1.0);
}

