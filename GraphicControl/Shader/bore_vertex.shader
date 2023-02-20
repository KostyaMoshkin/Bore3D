#version 460 core

layout(location = 0) in float m_fRadius;

layout(std430, binding = 0) buffer BufferDepth { float m_vDepth[];    };
layout(std430, binding = 1) buffer BufferAngle { float m_fAddAngle[]; };

uniform int m_nCurveCount;

struct VSOUT {
	float fRadius;
	float vDepth;
	float fAddAngle;
	uint nCurve;
};

out VSOUT vdata;

uint getCurve(uint nVertexId_)
{
	return nVertexId_ - (nVertexId_ / m_nCurveCount) * uint(m_nCurveCount);
}

int positionY()
{
	return gl_VertexID / m_nCurveCount;
}

void main()
{
	vdata.fRadius = m_fRadius;
	vdata.vDepth = m_vDepth[positionY()];
	vdata.fAddAngle = m_fAddAngle[positionY()];
	vdata.nCurve = getCurve(gl_VertexID - gl_BaseVertex);
}