#include "pch.h"
#include "BoreControl.h"

#include <gl.h>

namespace GraphicControl
{
    BoreControl::BoreControl()
        : ControlGL()
    {
        m_pRenderBoreSurface = GL::RenderBoreSurface::Create();
    }

    void BoreControl::paint()
	{
        if (!(m_bDataInit && m_bPaletteInit))
            return;

        m_pRenderBoreSurface->draw();

        needUpdate();

        return;
	}

    bool BoreControl::InitBore3D(DataProvider::IBoreData* pData, float fLogPerPixel)
    {
        needUpdate();

        m_pData = pData;

        m_bDataInit = true;
        return false;
    }

    bool BoreControl::InitPalette(const std::vector<COLORREF>& vecPalette)
    {
        needUpdate();
        m_bPaletteInit = true;
        return false;
    }

    int BoreControl::GetBitmap(const RECT* pVisualRect, DataProvider::IDiaMapper* pMapper, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        return 0;
    }
}