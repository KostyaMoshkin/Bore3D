#include "pch.h"
#include "BoreControl.h"

#include <gl.h>

namespace GraphicControl
{
    BoreControl::BoreControl()
        : ControlGL()
    {
        setBkgColor(0, 0, 0.3f);
    }

    bool BoreControl::paint()
	{
        if (m_fGreen < 1.0f)
            m_fGreen += 1.0f / 255 / 30;
        else 
            m_fGreen = 0;

        glColor3f(0.0f, m_fGreen, 1.0f); //blue color

        glBegin(GL_TRIANGLES);//start drawing triangles
        glVertex3f(-1.0f, -0.25f, 0.0f);//triangle one first vertex
        glVertex3f(-0.5f, -0.25f, 0.0f);//triangle one second vertex
        glVertex3f(-0.75f, 0.25f, 0.0f);//triangle one third vertex
        //drawing a new triangle

        glColor3f(0.0f, 0, m_fGreen); //blue color
        glVertex3f(0.5f, -0.25f, 0.0f);//triangle two first vertex
        glVertex3f(1.0f, -0.25f, 0.0f);//triangle two second vertex
        glVertex3f(0.75f, 0.25f, 0.0f);//triangle two third vertex
        glEnd();//end drawing of triangles

        glBegin(GL_POLYGON);
        glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
        glColor3f(1, m_fGreen, 1); glVertex3f(0.6, -0.75, 0.0);
        glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
        glEnd();

        glFlush();

        return false;
	}

    bool BoreControl::InitBore3D(IBoreData* pData, float fLogPerPixel)
    {
        return false;
    }

    bool BoreControl::InitPalette(const std::vector<COLORREF>& vecPalette)
    {
        return false;
    }

    int BoreControl::GetBitmap(const RECT* pVisualRect, IDiaMapper* pMapper, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        return 0;
    }
}