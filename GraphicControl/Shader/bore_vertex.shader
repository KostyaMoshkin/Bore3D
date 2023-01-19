#version 460 core

layout(location = 0) in float m_fValue;

layout(std430, binding = 0) buffer BufferDepth { float m_vDepth[]; };

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;

uniform float m_fTop;
uniform float m_fBottom;
uniform float m_fRotation;
uniform float m_fMinRadius;
uniform float m_fMaxRadius;

uniform int m_nMinRadiusLP;
uniform int m_nMaxRadiusLP;

uniform int m_nCurveCount;
uniform int m_nDriftCount;

uniform mat4 m_MVP;

smooth out float fPaletteIndex;

float Pi = 3.1415926;

float getAngle(uint nVertexId_)
{
	uint nDrift = nVertexId_ - (nVertexId_ / m_nDriftCount) * uint(m_nDriftCount);
	return Pi * 2.0 * float(nDrift) / float(m_nDriftCount);
}

float positionY(uint nVertexId_)
{
	if (nVertexId_ < m_nDriftCount)
		return gl_BaseVertex / m_nDriftCount;

	return gl_BaseVertex / m_nDriftCount + 1;
}

void main()
{
	uint nVertexId = gl_VertexID - gl_BaseVertex;
	uint nRawNumber = gl_VertexID / gl_BaseVertex;

	vec3 vPosition = vec3(
		m_fValue * sin(m_fRotation + getAngle(nVertexId)),
		m_vDepth[int(nRawNumber + positionY(nVertexId))],
		m_fValue * cos(m_fRotation + getAngle(nVertexId))
	);

	gl_Position = m_MVP * vec4(vPosition, 1.0);

	fPaletteIndex = (m_fValue - m_fPaletteValueMin) / (m_fPaletteValueMax - m_fPaletteValueMin);
}