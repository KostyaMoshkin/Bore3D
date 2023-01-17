#include "pch.h"
#include "RenderBoreSurface.h"
#include "BufferBounder.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

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
        if (!(m_bDataInit && m_bPaletteInit && m_bProgramInit))
            return;


        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //---------------------------------------------------------------------------

        drawTriangles();
        return;

        //---------------------------------------------------------------------------

        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        BufferBounder<TextureBuffer> textureBounder(m_pPaletteBuffer);
        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);

        Matrix4 mPRV(0);

        m_pShaderProgram->setUniformMat4f("m_MVP", &mPRV[0][0]);

        glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nCurveCount - 1,
            0);

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
            auto a = (vecPalette_[i] & rgbGreen);
            auto b = (vecPalette_[i] & rgbGreen) >> 8;
            auto c = (float)((vecPalette_[i] & rgbGreen) >> 8) / 255;

            vColorText[i][0] = (float)((vecPalette_[i] & rgbRed)       >> 0 ) / 255;
            vColorText[i][1] = (float)((vecPalette_[i] & rgbGreen)     >> 8 ) / 255;
            vColorText[i][2] = (float)((vecPalette_[i] & rgbBlue)      >> 16) / 255;
        }

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        BufferBounder<TextureBuffer> textureBounder(m_pPaletteBuffer);

        if (!m_pPaletteBuffer->fillBuffer1D(GL_RGB, vColorText.size(), GL_RGB, GL_FLOAT, vColorText.data()))
            return false;

        float fDataMin = 0;
        float fDataMax = 0;

        m_pShaderProgram->setUniform1f("m_fPaletteValueMin", &fDataMin);
        m_pShaderProgram->setUniform1f("m_fPaletteValueMax", &fDataMax);

        //----------------------------------------------------------------------------------

        renderBoreBounder.unbound();

        m_bPaletteInit = true;

        return true;
    }


    bool RenderBoreSurface::InitDiaMapper(void* pMapper_)
    {
        m_pImpl->pMapper = (DataProvider::IDiaMapper *)pMapper_;

        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
        {
            m_pImpl->vDepths[i] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetDepths().data()[i]);
            m_pImpl->vRotation[i] = (float)(m_pImpl->pData->GetRotation().data()[i]);

            for(int j = 0; j < m_pImpl->pData->GetRadiusCurve(0).size(); ++j)
                m_pImpl->vvRadiusCurve[i][j] = (float)(m_pImpl->pData->GetRadiusCurve(i).data()[j]);
        }

        //----------------------------------------------------------------------------------


        BufferBounder<ShaderProgram> programBounder(m_pShaderProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);

        m_bDataInit = true;

        if (!m_VertexBuffer->bookSpace(int(m_pImpl->nCurveCount * m_pImpl->nDriftCount * sizeof(float))))
            return false;

        {
            BufferMounter<VertexBuffer> vertexMounter(m_VertexBuffer);

            if (float* pPosition = (float*)vertexMounter.get_buffer())
                memcpy((void*)pPosition, (void*)m_pImpl->vvRadiusCurve[0].data(), m_pImpl->nCurveCount * m_pImpl->nDriftCount * sizeof(float));
            else
                return false;
        }

        m_pShaderProgram->setUniform1i("m_nCurveCount", &(m_pImpl->nCurveCount));
        m_pShaderProgram->setUniform1i("m_nDriftCount", &(m_pImpl->nDriftCount));

        //----------------------------------------------------------------------------------

        std::vector<DrawElementsIndirectCommand> vIndirectCommand(m_pImpl->nCurveCount - 1);
        for (int i = 0; i < m_pImpl->nCurveCount - 1; ++i)
        {
            vIndirectCommand[i].count = m_pImpl->nDriftCount * 2;
            vIndirectCommand[i].primCount = 1;
            vIndirectCommand[i].firstIndex = 0;
            vIndirectCommand[i].baseVertex = i * m_pImpl->nDriftCount;
            vIndirectCommand[i].baseInstance = 0;
        }

        BufferBounder<IndirectBuffer> indirectBounder(m_pBufferIndirect);

        if (!m_pBufferIndirect->bookSpace(int(vIndirectCommand.size() * sizeof(DrawElementsIndirectCommand))))
            return false;

        {
            BufferMounter<IndirectBuffer> indirectMounter(m_pBufferIndirect);

            if (void* pPosition = (void*)indirectMounter.get_buffer())
                memcpy((void*)pPosition, (void*)vIndirectCommand.data(), vIndirectCommand.size() * sizeof(DrawElementsIndirectCommand));
            else
                return false;
        }

        //if (!m_pBufferIndirect->fillBuffer(sizeof(DrawElementsIndirectCommand) * vIndirectCommand.size(), vIndirectCommand.data()))
        //    return false;


        //----------------------------------------------------------------------------------

        std::vector<unsigned int> indices(m_pImpl->nDriftCount * 2);

        for (unsigned int i = 0; i < m_pImpl->nDriftCount; ++i)
        {
            indices[i * 2] = m_pImpl->nDriftCount + i;
            indices[i * 2 + 1] = i;
        }

        BufferBounder<IndexBuffer> indexBounder(m_pBufferIndex);

        if (!m_pBufferIndex->bookSpace(vIndirectCommand.size() * sizeof(unsigned int)))
            return false;

        {
            BufferMounter<IndexBuffer> indexMounter(m_pBufferIndex);

            if (void* pPosition = (void*)indexMounter.get_buffer())
                memcpy((void*)pPosition, (void*)vIndirectCommand.data(), vIndirectCommand.size() * sizeof(unsigned int));
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

        m_pPaletteBuffer    = std::make_shared<TextureBuffer>(GL_TEXTURE_1D);
        m_VertexBuffer      = std::make_shared<VertexBuffer>();
        m_pBufferIndirect   = std::make_shared<IndirectBuffer>();
        m_pBufferIndex      = std::make_shared<IndexBuffer>();

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