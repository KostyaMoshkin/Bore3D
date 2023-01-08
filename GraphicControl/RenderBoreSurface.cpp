#include "pch.h"
#include "RenderBoreSurface.h"

namespace GL
{
	RenderBoreSurface::RenderBoreSurface()
	{
        setBkgColor(0, 0, 0.3f);
	}
	
	void RenderBoreSurface::draw()
	{
        if (m_fGreen < 1.0f)
            m_fGreen += 1.0f / 255 / 30;
        else
            m_fGreen = 0;

        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        glColor3f(1, 0, 0); glVertex3f(-0.6f, -0.75f, 0.5f);
        glColor3f(1, m_fGreen, 1); glVertex3f(0.6f, -0.75f, 0.0f);
        glColor3f(0, 0, 1); glVertex3f(0.0f, 0.75f, 0.0f);
        glEnd();

        glFlush();

	}

    void RenderBoreSurface::setBkgColor(float r_, float g_, float b_)
    {
        m_vBkgColor[0] = r_;
        m_vBkgColor[1] = g_;
        m_vBkgColor[2] = b_;
    }
    bool RenderBoreSurface::init()
    {
        return false;
    }
    void RenderBoreSurface::lookAt(Matrix4& mView_)
    {
    }
    void RenderBoreSurface::rotate(Matrix4& mRotate_)
    {
    }
    void RenderBoreSurface::translate(Matrix4& mTranslate_)
    {
    }
    void RenderBoreSurface::setViewAngle(Matrix4& mPerspective_)
    {
    }
    void RenderBoreSurface::bound()
    {
    }
    void RenderBoreSurface::unbound()
    {
    }
    float RenderBoreSurface::getScale()
    {
        return 0.0f;
    }
    void RenderBoreSurface::setScale(float fScale_)
    {
    }
}