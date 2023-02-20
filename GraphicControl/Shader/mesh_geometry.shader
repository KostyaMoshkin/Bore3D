#version 460 core
layout(lines_adjacency) in;
layout(line_strip, max_vertices = 150) out;

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;

uniform float m_fRotation;
uniform float m_fMinRadius;
uniform float m_fMaxRadius;
uniform float m_fIsometryAngle;

uniform int m_nMinRadiusLP;
uniform int m_nMaxRadiusLP;

uniform int m_nCurveCount;

uniform int m_nInterpolateCount;

uniform int m_nMeshStep;

uniform mat4 m_MVP;

smooth out float fPaletteIndex;

struct VSOUT {
    float fRadius;
    float vDepth;
    float fAddAngle;
    uint nCurve;
    uint nDepthIndex;
};

in VSOUT vdata[];

void EmitPoint(float fRadius_, float fAddAngle_, float vDepth_, float fDriftProportion_)
{
    float fRadius = m_nMinRadiusLP + (fRadius_ - m_fMinRadius) / (m_fMaxRadius - m_fMinRadius) * (m_nMaxRadiusLP - m_nMinRadiusLP);
    fRadius *= 1.002;  //  „то бы сетка была снаружи поверхности

    float fCurveAngle = radians(fAddAngle_ + m_fRotation + 360.0 * fDriftProportion_);

    vec3 vPosition = vec3(
        fRadius * sin(-1.0 * fCurveAngle),
        vDepth_,
        fRadius * cos(-1.0 * fCurveAngle)
    );

    gl_Position = m_MVP * vec4(vPosition, 1.0);
    gl_Position.y = gl_Position.y + gl_Position.z * sin(-m_fIsometryAngle);

    fPaletteIndex = (fRadius_ - m_fPaletteValueMin) / (m_fPaletteValueMax - m_fPaletteValueMin);

    EmitVertex();
}

void main()
{
    // max_vertices = 150; max nTriangleCount = max_vertices / 6 = 25
    int nTriangleCount = min(25, m_nInterpolateCount);

    EmitPoint(vdata[0].fRadius, vdata[0].fAddAngle, vdata[0].vDepth, float(vdata[0].nCurve) / float(m_nCurveCount));
    EmitPoint(vdata[1].fRadius, vdata[1].fAddAngle, vdata[1].vDepth, float(vdata[1].nCurve) / float(m_nCurveCount));

    if (vdata[1].nDepthIndex % m_nMeshStep == 0)
        for (uint i = 0; i < nTriangleCount; ++i)
        {
            float fRadius2 = vdata[2].fRadius + (vdata[3].fRadius - vdata[2].fRadius) * (i + 0) / nTriangleCount;
            float fRadius3 = vdata[2].fRadius + (vdata[3].fRadius - vdata[2].fRadius) * (i + 1) / nTriangleCount;

            float fProportion2 = (float(vdata[2].nCurve) + float(i + 0) / nTriangleCount) / float(m_nCurveCount);
            float fProportion3 = (float(vdata[2].nCurve) + float(i + 1) / nTriangleCount) / float(m_nCurveCount);

            //------------------------------------------------------------------------------------------

            EmitPoint(fRadius2, vdata[0].fAddAngle, vdata[2].vDepth, fProportion2);
            EmitPoint(fRadius3, vdata[0].fAddAngle, vdata[3].vDepth, fProportion3);
        }

    EndPrimitive();
}

