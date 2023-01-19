#include "pch.h"
#include "RenderBoreSurface.h"
#include "BufferBounder.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <limits>

//#define TEST_DRAW

namespace GL
{
    static void drawTriangles()
    {
        float fRandColor1 = (float)std::rand() / RAND_MAX;
        float fRandColor2 = (float)std::rand() / RAND_MAX;
        float fRandColor3 = (float)std::rand() / RAND_MAX;

        glBegin(GL_TRIANGLES);
        {
            glColor3f(fRandColor3, fRandColor1, fRandColor2);
            glVertex3f(-1.0f, -0.25f, 0.0f);
            glVertex3f(-0.5f, -0.25f, 0.0f);
            glVertex3f(-0.75f, 0.25f, 0.0f);

            glColor3f(fRandColor2, fRandColor3, fRandColor1);
            glVertex3f(0.5f, -0.25f, 0.0f);
            glVertex3f(1.0f, -0.25f, 0.0f);
            glVertex3f(0.75f, 0.25f, 0.0f);

            glColor3f(fRandColor1, fRandColor3, fRandColor2);
            glVertex3f(-0.6f, -0.75f, 0.5f);
            glColor3f(fRandColor3, fRandColor2, fRandColor1);
            glVertex3f(0.6f, -0.75f, 0.0f);
            glColor3f(fRandColor3, fRandColor1, fRandColor1);
            glVertex3f(0.0f, 0.75f, 0.0f);
        }
        glEnd();

        glFlush();
    }


    struct RenderBoreSurface::Implementation
    {
        DataProvider::BoreData* pData = nullptr;
        DataProvider::IDiaMapper* pMapper = nullptr;

        float fLogPerPixel;

        int nCurveCount;
        int nDriftCount;

        std::vector<float> vDepths;
        std::vector<float> vRadiusCurve;
        std::vector<float> vRotation;

        bool bIsDiameters;
    };

    RenderBoreSurface::RenderBoreSurface()
	{
        setBkgColor(0, 0, 0);

        std::srand((unsigned)std::time(nullptr));

        m_pImpl = std::make_shared<Implementation>();
    }
	
	void RenderBoreSurface::draw()
	{
        if (!(m_bDataInit && m_bPaletteInit && m_bProgramInit))
            return;


        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //---------------------------------------------------------------------------

#ifdef TEST_DRAW
        drawTriangles();
        return;
#endif

        //---------------------------------------------------------------------------

        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        BufferBounder<TextureBuffer> paletteBounder(m_pPaletteBuffer);
        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);
        BufferBounder<ShaderStorageBuffer> depthBounder(m_pBufferDepth);
        BufferBounder<IndirectBuffer> indirectBounder(m_pBufferIndirect);

        glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nCurveCount - 1,
            0);

        renderBoreBounder.unbound();
	}

    bool RenderBoreSurface::InitBore3D(void* pData_, float fLogPerPixel_)
    {
        m_pImpl->pData = (DataProvider::BoreData*) pData_;

        m_pImpl->nCurveCount = m_pImpl->pData->GetCurveCount();

        if (m_pImpl->nCurveCount > 0)
            m_pImpl->nDriftCount = (int)m_pImpl->pData->GetRadiusCurve(0).size();
        else
            return false;

        m_pImpl->bIsDiameters = m_pImpl->pData->IsDiameters();

        //----------------------------------------------------------------------------------

        m_pImpl->vDepths.resize(m_pImpl->nCurveCount); 
        m_pImpl->vRotation.resize(m_pImpl->nCurveCount);

        m_pImpl->vRadiusCurve.resize(m_pImpl->nCurveCount * m_pImpl->nDriftCount);

        //----------------------------------------------------------------------------------

        return false;
    }

    bool RenderBoreSurface::InitPalette(const std::vector<COLORREF>& vecPalette_)
    {
        const COLORREF rgbRed   = 0x000000FF;
        const COLORREF rgbGreen = 0x0000FF00;
        const COLORREF rgbBlue  = 0x00FF0000;

        std::vector<float[3]> vPalette(vecPalette_.size());
        for (int i = 0; i < vPalette.size(); ++i)
        {
            auto a = (vecPalette_[i] & rgbGreen);
            auto b = (vecPalette_[i] & rgbGreen) >> 8;
            auto c = (float)((vecPalette_[i] & rgbGreen) >> 8) / 255;

            vPalette[i][0] = (float)((vecPalette_[i] & rgbRed)       >> 0 ) / 255;
            vPalette[i][1] = (float)((vecPalette_[i] & rgbGreen)     >> 8 ) / 255;
            vPalette[i][2] = (float)((vecPalette_[i] & rgbBlue)      >> 16) / 255;
        }

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);
        BufferBounder<TextureBuffer> paletteBounder(m_pPaletteBuffer);

        if (!m_pPaletteBuffer->fillBuffer1D(GL_RGB, vPalette.size(), GL_RGB, GL_FLOAT, vPalette.data()))
            return false;

        //----------------------------------------------------------------------------------

        renderBoreBounder.unbound();

        m_bPaletteInit = true;

        return true;
    }


    bool RenderBoreSurface::InitDiaMapper(void* pMapper_)
    {
        float fRadiusMin = std::numeric_limits<float>::max();
        float fRadiusMax = -std::numeric_limits<float>::max();

        m_fDepthMin = std::numeric_limits<float>::max();
        m_fDepthMax = -std::numeric_limits<float>::max();

        m_pImpl->pMapper = (DataProvider::IDiaMapper *)pMapper_;

        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
        {
            m_pImpl->vDepths[i] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetDepths().data()[i]);
            m_fDepthMin = std::min(m_fDepthMin, m_pImpl->vDepths[i]);
            m_fDepthMax = std::max(m_fDepthMax, m_pImpl->vDepths[i]);

            m_pImpl->vRotation[i] = (float)(m_pImpl->pData->GetRotation().data()[i]);

            for (int j = 0; j < m_pImpl->nDriftCount; ++j)
            {
                m_pImpl->vRadiusCurve[i * m_pImpl->nDriftCount + j] = (float)(m_pImpl->pData->GetRadiusCurve(i).data()[j]);
                fRadiusMin = std::min(fRadiusMin, m_pImpl->vRadiusCurve[i * m_pImpl->nDriftCount + j]);
                fRadiusMax = std::max(fRadiusMax, m_pImpl->vRadiusCurve[i * m_pImpl->nDriftCount + j]);
            }
        }

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        m_pShaderProgram->setUniform1f("m_fPaletteValueMin", &fRadiusMin);
        m_pShaderProgram->setUniform1f("m_fPaletteValueMax", &fRadiusMax);

        //----------------------------------------------------------------------------------

        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);

        int nVertexBufferSize = int(m_pImpl->nCurveCount * m_pImpl->nDriftCount * sizeof(float));

        if (!m_VertexBuffer->bookSpace(nVertexBufferSize) )
            return false;

        {
            BufferMounter<VertexBuffer> vertexMounter(m_VertexBuffer);

            if (void* pPosition = vertexMounter.get_buffer())
                memcpy(pPosition, m_pImpl->vRadiusCurve.data(), nVertexBufferSize);
            else
                return false;
        }

        m_VertexBuffer->attribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

        m_pShaderProgram->setUniform1i("m_nCurveCount", &(m_pImpl->nCurveCount));
        m_pShaderProgram->setUniform1i("m_nDriftCount", &(m_pImpl->nDriftCount));

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderStorageBuffer> depthBounder(m_pBufferDepth);

        int nBufferDepthSize = int(m_pImpl->nCurveCount * sizeof(float));

        if (!m_pBufferDepth->bookSpace(nBufferDepthSize))
            return false;

        {
            BufferMounter<ShaderStorageBuffer> depthMounter(m_pBufferDepth);

            if (void* pPosition = depthMounter.get_buffer())
                memcpy(pPosition, m_pImpl->vDepths.data(), nBufferDepthSize);
            else
                return false;
        }


        //----------------------------------------------------------------------------------

        std::vector<DrawElementsIndirectCommand> vIndirectCommand(m_pImpl->nCurveCount - 1);
        for (int i = 0; i < m_pImpl->nCurveCount - 1; ++i)
        {
            vIndirectCommand[i].count = m_pImpl->nDriftCount * 2 + 2;  // +2 т к надо замкнуть окружность, последн€€ точка соедин€етс€ с первой
            vIndirectCommand[i].primCount = 1;
            vIndirectCommand[i].firstIndex = 0;
            vIndirectCommand[i].baseVertex = i * m_pImpl->nDriftCount;
            vIndirectCommand[i].baseInstance = 0;
        }

        BufferBounder<IndirectBuffer> indirectBounder(m_pBufferIndirect);

        int nBufferIndirectSize = int(vIndirectCommand.size() * sizeof(DrawElementsIndirectCommand));

        if (!m_pBufferIndirect->bookSpace(nBufferIndirectSize))
            return false;

        {
            BufferMounter<IndirectBuffer> indirectMounter(m_pBufferIndirect);

            if (void* pPosition = indirectMounter.get_buffer())
                memcpy(pPosition, vIndirectCommand.data(), nBufferIndirectSize);
            else
                return false;
        }

        //----------------------------------------------------------------------------------

        std::vector<unsigned int> vIndices(m_pImpl->nDriftCount * 2 + 2);

        for (unsigned int i = 0; i < (unsigned int)m_pImpl->nDriftCount; ++i)
        {
            vIndices[i * 2] = m_pImpl->nDriftCount + i;
            vIndices[i * 2 + 1] = i;
        }

        //  Ќеобходимо домолнить индексы, чтобы последн€€ точка в р€ду замкнулась с первой
        vIndices[m_pImpl->nDriftCount * 2] = m_pImpl->nDriftCount;
        vIndices[m_pImpl->nDriftCount * 2 + 1] = 0;


        BufferBounder<IndexBuffer> indexBounder(m_pBufferIndex);

        int nBufferIndexSize = (int)vIndices.size() * sizeof(unsigned int);

        if (!m_pBufferIndex->bookSpace(nBufferIndexSize))
            return false;

        {
            BufferMounter<IndexBuffer> indexMounter(m_pBufferIndex);

            if (void* pPosition = indexMounter.get_buffer())
                memcpy(pPosition, vIndices.data(), nBufferIndexSize);
            else
                return false;
        }

        //----------------------------------------------------------------------------------

        renderBoreBounder.unbound();

        m_bDataInit = true;

        return true;
    }


    int RenderBoreSurface::GetBitmap(const RECT* pVisualRect, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        m_pShaderProgram->setUniform1f("m_fTop", &fTop);
        m_pShaderProgram->setUniform1f("m_fBottom", &fBottom);
        m_pShaderProgram->setUniform1f("m_fRotation", &fRotation);
        m_pShaderProgram->setUniform1f("m_fMinRadius", &fMinRadius);
        m_pShaderProgram->setUniform1f("m_fMaxRadius", &fMaxRadius);
        m_pShaderProgram->setUniform1i("m_nMinRadiusLP", &nMinRadiusLP);
        m_pShaderProgram->setUniform1i("m_nMaxRadiusLP", &nMaxRadiusLP);

        m_mPRV = glm::ortho(-fMaxRadius, fMaxRadius, m_fDepthMin, m_fDepthMax, -fMaxRadius, fMaxRadius);
        m_pShaderProgram->setUniformMat4f("m_MVP", &m_mPRV[0][0]);

        renderBoreBounder.unbound();

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
        int nVersionSupported = getVersionGl();

        const GLubyte* pVersion = glGetString(GL_VERSION);

        if (!!m_pShaderProgram)
            return true;

        ShaderProgramPtr pShaderProgram = std::make_shared<ShaderProgram>();

        bool bAddShader = false;

        bAddShader |= !pShaderProgram->addShader(ShaderName::bore_fragment, ShaderProgram::ShaderType::Fragment());
        bAddShader |= !pShaderProgram->addShader(ShaderName::bore_vertex, ShaderProgram::ShaderType::Vertex());

        if (bAddShader)
            return false;

        if (!pShaderProgram->init())
            return false;

        glGenVertexArrays(1, &m_nVAO);

        m_pShaderProgram = pShaderProgram;

        //----------------------------------------------------------------------------------

        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        m_VertexBuffer      = std::make_shared<VertexBuffer>();
        m_pBufferIndirect   = std::make_shared<IndirectBuffer>();
        m_pBufferIndex      = std::make_shared<IndexBuffer>();
        m_pBufferDepth      = std::make_shared<ShaderStorageBuffer>(0);

        m_pPaletteBuffer    = std::make_shared<TextureBuffer>(GL_TEXTURE_1D, GL_TEXTURE0, GL_LINEAR);
        m_pPaletteBuffer->alignment(1);

        m_bProgramInit = true;

        return true;
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
        glBindVertexArray(m_nVAO);
    }

    void RenderBoreSurface::unbound()
    {
        glBindVertexArray(0);
    }
}