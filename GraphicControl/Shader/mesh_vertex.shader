#version 460 core

layout(location = 0) in float m_fRadius;

layout(std430, binding = 0) buffer BufferDepth { float m_vDepth[]; };
layout(std430, binding = 1) buffer BufferAngle { float m_fAddAngle[]; };

uniform float m_fRotation;
uniform float m_fMinRadius;
uniform float m_fMaxRadius;
uniform float m_fIsometryAngle;

uniform int m_nMinRadiusLP;
uniform int m_nMaxRadiusLP;

uniform int m_nCurveCount;

uniform mat4 m_MVP;

float Pi = 3.1415926;

uint getDrift(uint nVertexId_)
{
	return nVertexId_ - (nVertexId_ / m_nCurveCount) * uint(m_nCurveCount);
}

float getAngle(uint nVertexId_)
{
	return 360.0 * float(getDrift(nVertexId_)) / float(m_nCurveCount);
}

int positionY()
{
	return gl_VertexID / m_nCurveCount;
}

void main()
{
	uint nVertexId = gl_VertexID - gl_BaseVertex;

	float fRadius = m_nMinRadiusLP + (m_fRadius - m_fMinRadius) / (m_fMaxRadius - m_fMinRadius) * (m_nMaxRadiusLP - m_nMinRadiusLP);
	fRadius *= 1.001;

	float fRriftAndleGrad = m_fAddAngle[positionY()] + m_fRotation + getAngle(nVertexId);
	float fDriftAngle = Pi * 2.0 / 360.0 * fRriftAndleGrad;

	vec3 vPosition = vec3(
		fRadius * sin(-1.0 * fDriftAngle),
		m_vDepth[positionY()],
		fRadius * cos(-1.0 * fDriftAngle)
	);

	gl_Position = m_MVP * vec4(vPosition, 1.0);

	gl_Position = vec4(gl_Position.x, gl_Position.y + gl_Position.z * sin(-m_fIsometryAngle), gl_Position.z, gl_Position.w);
}