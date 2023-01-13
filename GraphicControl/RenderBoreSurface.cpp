#include "pch.h"
#include "RenderBoreSurface.h"
#include "BufferBounder.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

namespace GL
{

    struct RenderBoreSurface::Implementation
    {
        DataProvider::BoreData* pData = nullptr;
        DataProvider::IDiaMapper* pMapper = nullptr;

        float fLogPerPixel;

        int nCurveCount;
        int nDriftCount;

        std::vector<float> vDepths;
        std::vector<std::vector<float>> vvRadiusCurve;
        std::vector<float> vRotation;

        bool bIsDiameters;
    };

    RenderBoreSurface::RenderBoreSurface()
	{
        setBkgColor(0, 0, 0.3f);

        std::srand((unsigned)std::time(nullptr));

        m_pImpl = std::make_shared<Implementation>();
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

    bool RenderBoreSurface::InitBore3D(void* pData_, float fLogPerPixel_)
    {
        m_pImpl->pData = (DataProvider::BoreData*) pData_;

        m_pImpl->nCurveCount = m_pImpl->pData->GetCurveCount();  /// !!!
        m_pImpl->bIsDiameters = m_pImpl->pData->IsDiameters();

        //----------------------------------------------------------------------------------

        m_pImpl->vDepths.resize(m_pImpl->nCurveCount); 
        m_pImpl->vRotation.resize(m_pImpl->nCurveCount);

        m_pImpl->vvRadiusCurve.resize(m_pImpl->nCurveCount); 
        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
            m_pImpl->vvRadiusCurve[i].resize(m_pImpl->pData->GetRadiusCurve(0).size());


        //----------------------------------------------------------------------------------

        return false;
    }

    bool RenderBoreSurface::InitPalette(const std::vector<COLORREF>& vecPalette_)
    {
        const COLORREF rgbRed   = 0x000000FF;
        const COLORREF rgbGreen = 0x0000FF00;
        const COLORREF rgbBlue  = 0x00FF0000;

        std::vector<float[3]> vColorText(vecPalette_.size());
        for (int i = 0; i < vColorText.size(); ++i)
        {
            vColorText[i][0] = (float)((vecPalette_[i] && rgbRed)       >> 0 ) / 255;
            vColorText[i][1] = (float)((vecPalette_[i] && rgbGreen)     >> 8 ) / 255;
            vColorText[i][2] = (float)((vecPalette_[i] && rgbBlue)      >> 16) / 255;
        }

        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> sectorBounder(this);

        if (m_pPaletteBuffer == nullptr)
            m_pPaletteBuffer = std::make_shared<TextureBuffer>(GL_TEXTURE_1D);

        BufferBounder<TextureBuffer> textureBounder(m_pPaletteBuffer);

        if (!m_pPaletteBuffer->fillBuffer1D(GL_RGB, vColorText.size(), GL_RGB, GL_FLOAT, vColorText.data()))
            return false;

        //m_pShaderProgram->setUniform1f("m_fPaletteValueMin", &fDataMin_);
        //m_pShaderProgram->setUniform1f("m_fPaletteValueMax", &fDataMax_);

        return false;
    }

    int RenderBoreSurface::GetBitmap(const RECT* pVisualRect, void* pMapper, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        m_pImpl->pMapper = (DataProvider::IDiaMapper *)pMapper;

        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
        {
            m_pImpl->vDepths[i] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetDepths().data()[i]);
            m_pImpl->vRotation[i] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetRotation().data()[i]);

            for(int j = 0; j < m_pImpl->pData->GetRadiusCurve(0).size(); ++j)
                m_pImpl->vvRadiusCurve[i][j] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetRadiusCurve(i).data()[i]);
        }

        return 0;
    }

    void RenderBoreSurface::setBkgColor(float r_, float g_, float b_)
    {
        m_vBkgColor[0] = r_;
        m_vBkgColor[1] = g_;
        m_vBkgColor[2] = b_;
    }
    bool RenderBoreSurface::init()
    {
        if (!!m_pShaderProgram)
            return true;

        ShaderProgramPtr pShaderProgram = std::make_shared<ShaderProgram>();

        pShaderProgram->addShader(ShaderName::bore_vertex, ShaderProgram::ShaderType::Vertex());
        pShaderProgram->addShader(ShaderName::bore_fragment, ShaderProgram::ShaderType::Fragment());

        if (!pShaderProgram->init())
            return false;

        glGenVertexArrays(1, &m_nVAO);

        m_pShaderProgram = pShaderProgram;

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