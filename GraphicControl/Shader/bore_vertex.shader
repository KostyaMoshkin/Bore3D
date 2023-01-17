#version 460 core
layout(location = 0) in float m_fValue;

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;

uniform float m_fTop;
uniform float m_fBottom;
uniform float m_fRotation;
uniform float m_fMinRadius;
uniform float m_fMaxRadius;

uniform int m_nMinRadiusLP;
uniform int m_nMaxRadiusLP;

uniform mat4 m_MVP;

smooth out float fPaletteIndex;

void main()
{
	uint nVertexId = gl_VertexID - gl_BaseVertex;

	vec3 vPosition = vec3(0, 0, 0);
	gl_Position = m_MVP * vec4(vPosition, 1.0);

	fPaletteIndex = (m_fValue - m_fPaletteValueMin) / (m_fPaletteValueMax - m_fPaletteValueMin);
	fPaletteIndex = max(min(fPaletteIndex, 0.9999), 0.0001);
}