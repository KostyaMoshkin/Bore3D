#include "pch.h"
#include "BoreControl.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

namespace GraphicControl
{

    struct BoreControl::Implementation
    {
        DataProvider::BoreData* m_pData = nullptr;
    };

    BoreControl::BoreControl()
        : ControlGL()
    {
        m_pImpl = std::make_shared<Implementation>();
        m_pRenderBoreSurface = GL::RenderBoreSurface::Create();
    }

    BoreControl::~BoreControl()
    {
    }

    void BoreControl::paint()
	{
        if (!(m_bDataInit && m_bPaletteInit))
            return;

        m_pRenderBoreSurface->draw();

        needUpdate();

        return;
	}

    bool BoreControl::InitBore3D(void* pData, float fLogPerPixel)
    {
        needUpdate();

        m_pImpl->m_pData = (DataProvider::BoreData* )pData;

        m_bDataInit = true;
        return false;
    }

    bool BoreControl::InitPalette(const std::vector<COLORREF>& vecPalette)
    {
        needUpdate();
        m_bPaletteInit = true;
        return false;
    }

    int BoreControl::GetBitmap(const RECT* pVisualRect, void* pMapper, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        (DataProvider::IDiaMapper*)pMapper;
        return 0;
    }
}