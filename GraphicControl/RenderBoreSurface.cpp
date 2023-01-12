#include "pch.h"
#include "RenderBoreSurface.h"

namespace GL
{
	RenderBoreSurface::RenderBoreSurface()
	{
        setBkgColor(0, 0, 0.3f);

        std::srand((unsigned)std::time(nullptr));
    }
	
	void RenderBoreSurface::draw()
	{
        float fRandColor1 = (float)std::rand() / RAND_MAX;
        float fRandColor2 = (float)std::rand() / RAND_MAX;
        float fRandColor3 = (float)std::rand() / RAND_MAX;

        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(fRandColor3, fRandColor1, fRandColor2);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(-1.0f, -0.25f, 0.0f);
            glVertex3f(-0.5f, -0.25f, 0.0f);
            glVertex3f(-0.75f, 0.25f, 0.0f);

            glColor3f(fRandColor2, fRandColor3, fRandColor1);
            glVertex3f(0.5f, -0.25f, 0.0f);
            glVertex3f(1.0f, -0.25f, 0.0f);
            glVertex3f(0.75f, 0.25f, 0.0f);
            glEnd();

            glBegin(GL_POLYGON);
            glColor3f(fRandColor1, fRandColor3, 1); glVertex3f(-0.6f, -0.75f, 0.5f);
            glColor3f(1, fRandColor2, 1); glVertex3f(0.6f, -0.75f, 0.0f);
            glColor3f(fRandColor3, 0, fRandColor3); glVertex3f(0.0f, 0.75f, 0.0f);
        }
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