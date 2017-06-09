#version 100
precision mediump float;
// Uniforms
uniform mat4 uProjectionMatrix;
uniform float uK;
uniform float uTime;
// Attributes
attribute float aTheta;
attribute vec3 aShade;
// Outputs
varying vec3 vShade;

void main()
{
	float x = uTime * 1;//cos(uK*aTheta) * sin(aTheta);
	float y = uTime * 1;//cos(uK*aTheta) * cos(aTheta);
	
	gl_Position = uProjectionMatrix * vec4(x, y, 0.0, 1.0);
	gl_PointSize = 16.0;
	
	vShade = aShade;
}
