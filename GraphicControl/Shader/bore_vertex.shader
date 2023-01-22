#version 460 core

layout(location = 0) in float m_fRadius;

layout(std430, binding = 0) buffer BufferDepth { float m_vDepth[];    };
layout(std430, binding = 1) buffer BufferAngle { float m_fAddAngle[]; };

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;

uniform float m_fTop;
uniform float m_fBottom;
uniform float m_fRotation;
uniform float m_fMinRadius;
uniform float m_fMaxRadius;
uniform float m_fIsometryAngle;

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


int positionY()
{
	return gl_VertexID / m_nDriftCount;
}

void main()
{
	uint nVertexId = gl_VertexID - gl_BaseVertex;

	float fRadius = m_fRadius;

	vec3 vPosition = vec3(
		fRadius * sin(-1.0 * (m_fAddAngle[positionY()] + m_fRotation + getAngle(nVertexId))),
		m_vDepth[positionY()],
		fRadius * cos(-1.0 * (m_fAddAngle[positionY()] + m_fRotation + getAngle(nVertexId)))
	);

	gl_Position = m_MVP * vec4(vPosition, 1.0);

	gl_Position = vec4(gl_Position.x, gl_Position.y * cos(-m_fIsometryAngle) + gl_Position.z * sin(-m_fIsometryAngle), gl_Position.z, gl_Position.w);

	fPaletteIndex = (m_fRadius - m_fPaletteValueMin) / (m_fPaletteValueMax - m_fPaletteValueMin);
}