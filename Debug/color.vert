//
// vertex shader: WS 22/23
// Name:
// Matrikelnummer:
//

#version 420 core

// define location for vertex arrays - position and color
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

// matrix variables to be controlled from the application
uniform mat4 Model;
uniform mat4 Camera;
uniform mat4 Projection;

// output value vertex shader => fragment shader
out vec4 vertexColor;

void main(void)
{
	// ... need to be implemented
	gl_Position = Projection * Camera * Model * vPosition;
	vertexColor = vColor;

}
