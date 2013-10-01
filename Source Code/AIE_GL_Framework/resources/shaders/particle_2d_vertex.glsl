#version 400

in vec4 Position;
in vec2 Size;
in float Alpha;

out vec4 vWorldPosition;
out vec4 vScreenPosition;
out vec2 vSize;
out float vAlpha;

uniform sampler2D diffuseTexture;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	vScreenPosition	= Projection * View * Model * Position;
	vWorldPosition	= Position;
	vSize	= Size;
	vAlpha	= Alpha;
}
