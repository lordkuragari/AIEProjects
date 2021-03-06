#version 400

layout( triangles, equal_spacing, ccw ) in;

in vec4 tcWorldPosition[];
in vec2 tcUV[];

out vec4 teWorldPosition;
out vec2 teUV;
out vec4 teNormal;

uniform sampler2D displacementMap;

void main()
{
	vec4 norm = vec4( cross( tcWorldPosition[0].xyz - tcWorldPosition[1].xyz,
						tcWorldPosition[2].xyz - tcWorldPosition[0].xyz ), 0 );
	teNormal = normalize( norm );

	vec4 p0 = gl_in[ 0 ].gl_Position;
	vec4 p1 = gl_in[ 1 ].gl_Position;
	vec4 p2 = gl_in[ 2 ].gl_Position;
	vec3 p = gl_TessCoord.xyz;

	teUV = tcUV[0]*p.x + tcUV[1]*p.y + tcUV[2]*p.z;
	teWorldPosition = tcWorldPosition[0]*p.x + tcWorldPosition[1]*p.y + tcWorldPosition[2]*p.z;

	gl_Position = p0 * p.x + p1 * p.y + p2 * p.z;
}