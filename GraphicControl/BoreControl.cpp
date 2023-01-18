#include "pch.h"
#include "BoreControl.h"
#include "RenderBoreSurface.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

namespace GraphicControl
{
    struct BoreControl::Implementation
    {
        GL::RenderBoreSurfacePtr pRenderBoreSurface;
    };

    //-------------------------------------------------------------------------------------

    BoreControl::BoreControl()
        : ControlGL()
    {
        m_pImpl = std::make_shared<Implementation>();
        m_pImpl->pRenderBoreSurface = GL::RenderBoreSurface::Create();
    }

    BoreControl::~BoreControl()
    {
    }

    void BoreControl::paint()
	{
        m_pImpl->pRenderBoreSurface->draw();

        needUpdate();

        return;
	}

    bool BoreControl::init()
    {
        ControlGL::init();


        if (!beginDraw())
            return false;

        bool bResult = m_pImpl->pRenderBoreSurface->init();
        endDraw();

        return bResult;
    }

    bool BoreControl::InitBore3D(void* pData, float fLogPerPixel)
    {
        needUpdate();

        if (!beginDraw())
            return false;

        m_pImpl->pRenderBoreSurface->InitBore3D(pData, fLogPerPixel);
        endDraw();

        return false;
    }

    void BoreControl::InitDiaMapper(void* pMapper_)
    {
        if (!beginDraw())
            return;

        m_pImpl->pRenderBoreSurface->InitDiaMapper(pMapper_);
        endDraw();
    }

    bool BoreControl::InitPalette(const std::vector<COLORREF>& vecPalette)
    {
        needUpdate();

        if (!beginDraw())
            return false;

        m_pImpl->pRenderBoreSurface->InitPalette(vecPalette);
        endDraw();

        return false;
    }

    int BoreControl::GetBitmap(const RECT* pVisualRect, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        if (!beginDraw())
            return 0;

        m_pImpl->pRenderBoreSurface->GetBitmap(pVisualRect, fTop, fBottom, fRotation, fMinRadius, fMaxRadius, nMinRadiusLP, nMaxRadiusLP, fIsometryAngle, bDrawMesh);

        endDraw();

        return 0;
    }
}