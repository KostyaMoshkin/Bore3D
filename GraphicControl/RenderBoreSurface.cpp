#include "pch.h"
#include "RenderBoreSurface.h"
#include "BufferBounder.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <limits>

namespace GL
{
    struct RenderBoreSurface::Implementation
    {
        IBoreData* pData = nullptr;
        IDiaMapper* pMapper = nullptr;

        float fLogPerPixel;

        int nCurveCount;
        int nDepthCount;

        std::vector<float> vDepths;
        std::vector<float> vRadiusCurve;
        std::vector<float> vRotation;

        bool bIsDiameters;
    };

    RenderBoreSurface::RenderBoreSurface()
	{
        std::srand((unsigned)std::time(nullptr));

        m_pImpl = std::make_shared<Implementation>();
    }
	
    bool RenderBoreSurface::setSufraceProgram()
    {
        ShaderProgramPtr pSufraceProgram = ShaderProgram::Create();

        bool bAddSufraceShaderError = false;

        bAddSufraceShaderError |= !pSufraceProgram->addShader(ShaderName::bore_fragment, ShaderProgram::ShaderType::Fragment());
        bAddSufraceShaderError |= !pSufraceProgram->addShader(ShaderName::bore_vertex, ShaderProgram::ShaderType::Vertex());

        if (bAddSufraceShaderError)
            return false;

        if (!pSufraceProgram->init())
            return false;

        m_pSufraceProgram = pSufraceProgram;

        return true;
    }

    bool RenderBoreSurface::setVertexBuffer()
    {
        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);

        int nVertexBufferSize = int(m_pImpl->nCurveCount * m_pImpl->nDepthCount * sizeof(float));

        if (!m_VertexBuffer->bookSpace(nVertexBufferSize))
            return false;

        {
            BufferMounter<VertexBuffer> vertexMounter(m_VertexBuffer);

            if (void* pPosition = vertexMounter.get_buffer())
                memcpy(pPosition, m_pImpl->vRadiusCurve.data(), nVertexBufferSize);
            else
                return false;
        }

        m_VertexBuffer->attribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

        return true;
    }

    bool RenderBoreSurface::setSurfaceIndirect()
    {
        std::vector<DrawElementsIndirectCommand> vSurfaceIndirect(m_pImpl->nDepthCount);
        for (int i = 0; i < m_pImpl->nDepthCount; ++i)
        {
            vSurfaceIndirect[i].count = (GLuint)m_pImpl->nCurveCount * 2 + 2;
            vSurfaceIndirect[i].primCount = 1;
            vSurfaceIndirect[i].firstIndex = 0;
            vSurfaceIndirect[i].baseVertex = i * m_pImpl->nCurveCount;
            vSurfaceIndirect[i].baseInstance = 0;
        }

        BufferBounder<IndirectBuffer> indirectSurfaceBounder(m_pSurfaceIndirect);

        int nSurfaceIndirectSize = int(vSurfaceIndirect.size() * sizeof(DrawElementsIndirectCommand));

        if (!m_pSurfaceIndirect->bookSpace(nSurfaceIndirectSize))
            return false;

        {
            BufferMounter<IndirectBuffer> indirectMounter(m_pSurfaceIndirect);

            if (void* pPosition = indirectMounter.get_buffer())
                memcpy(pPosition, vSurfaceIndirect.data(), nSurfaceIndirectSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setSurfaceIndex()
    {
        std::vector<unsigned int> vSurfaceIndices(m_pImpl->nCurveCount * 2 + 2);  // +2 т к надо замкнуть окружность, последн€€ точка соедин€етс€ с первой

        for (unsigned int i = 0; i < (unsigned int)m_pImpl->nCurveCount; ++i)
        {
            vSurfaceIndices[i * 2] = m_pImpl->nCurveCount + i;
            vSurfaceIndices[i * 2 + 1] = i;
        }

        //  Ќеобходимо домолнить индексы, чтобы последн€€ точка в р€ду замкнулась с первой
        vSurfaceIndices[m_pImpl->nCurveCount * 2 + 0] = m_pImpl->nCurveCount;
        vSurfaceIndices[m_pImpl->nCurveCount * 2 + 1] = 0;

        BufferBounder<IndexBuffer> indexSurfaceBounder(m_pSurfaceIndex);

        int nSurfaceIndexSize = (int)vSurfaceIndices.size() * sizeof(unsigned int);

        if (!m_pSurfaceIndex->bookSpace(nSurfaceIndexSize))
            return false;

        {
            BufferMounter<IndexBuffer> indexSurfaceMounter(m_pSurfaceIndex);

            if (void* pPosition = indexSurfaceMounter.get_buffer())
                memcpy(pPosition, vSurfaceIndices.data(), nSurfaceIndexSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setBufferDepth()
    {
        BufferBounder<ShaderStorageBuffer> depthBounder(m_pBufferDepth);

        int nBufferDepthSize = int(m_pImpl->nDepthCount * sizeof(float));

        if (!m_pBufferDepth->bookSpace(nBufferDepthSize))
            return false;

        {
            BufferMounter<ShaderStorageBuffer> depthMounter(m_pBufferDepth);

            if (void* pPosition = depthMounter.get_buffer())
                memcpy(pPosition, m_pImpl->vDepths.data(), nBufferDepthSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setBufferAngle()
    {
        BufferBounder<ShaderStorageBuffer> angleBounder(m_pBufferAngle);

        int nBufferAngleSize = int(m_pImpl->nDepthCount * sizeof(float));

        if (!m_pBufferAngle->bookSpace(nBufferAngleSize))
            return false;

        {
            BufferMounter<ShaderStorageBuffer> angleMounter(m_pBufferAngle);

            if (void* pPosition = angleMounter.get_buffer())
                memcpy(pPosition, m_pImpl->vRotation.data(), nBufferAngleSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setMeshProgram()
    {
        ShaderProgramPtr pMeshProgram = ShaderProgram::Create();

        bool bAddMeshShaderError = false;

        bAddMeshShaderError |= !pMeshProgram->addShader(ShaderName::mesh_fragment, ShaderProgram::ShaderType::Fragment());
        bAddMeshShaderError |= !pMeshProgram->addShader(ShaderName::mesh_vertex, ShaderProgram::ShaderType::Vertex());

        if (bAddMeshShaderError)
            return false;

        if (!pMeshProgram->init())
            return false;

        m_pMeshProgram = pMeshProgram;

        return true;
    }

    bool RenderBoreSurface::setMeshIndirect()
    {
        std::vector<DrawElementsIndirectCommand> vMeshIndirect(m_pImpl->nDepthCount - 1);
        for (int i = 0; i < m_pImpl->nDepthCount - 1; ++i)
        {
            vMeshIndirect[i].count = (GLuint)m_pImpl->nCurveCount * 4;
            vMeshIndirect[i].primCount = 1;
            vMeshIndirect[i].firstIndex = 0;
            vMeshIndirect[i].baseVertex = i * m_pImpl->nCurveCount;
            vMeshIndirect[i].baseInstance = 0;
        }

        BufferBounder<IndirectBuffer> indirectMeshBounder(m_pMeshIndirect);

        int nMeshIndirectSize = int(vMeshIndirect.size() * sizeof(DrawElementsIndirectCommand));

        if (!m_pMeshIndirect->bookSpace(nMeshIndirectSize))
            return false;

        {
            BufferMounter<IndirectBuffer> indirectMounter(m_pMeshIndirect);

            if (void* pPosition = indirectMounter.get_buffer())
                memcpy(pPosition, vMeshIndirect.data(), nMeshIndirectSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setMeshIndex()
    {
        std::vector<unsigned int> vMeshIndices(m_pImpl->nCurveCount * 4);

        for (unsigned int i = 0; i < (unsigned int)m_pImpl->nCurveCount; ++i)
        {
            vMeshIndices[i * 4 + 0] = (GLuint)m_pImpl->nCurveCount + i;
            vMeshIndices[i * 4 + 1] = i;
            vMeshIndices[i * 4 + 2] = i;
            vMeshIndices[i * 4 + 3] = i + 1;
        }

        //  Ќеобходимо домолнить индексы, чтобы последн€€ точка в р€ду замкнулась с первой
        vMeshIndices[(m_pImpl->nCurveCount - 1) * 4 + 3] = 0;

        BufferBounder<IndexBuffer> indexMeshBounder(m_pMeshIndex);

        int nMeshIndexSize = (int)vMeshIndices.size() * sizeof(unsigned int);

        if (!m_pMeshIndex->bookSpace(nMeshIndexSize))
            return false;

        {
            BufferMounter<IndexBuffer> indexMeshMounter(m_pMeshIndex);

            if (void* pPosition = indexMeshMounter.get_buffer())
                memcpy(pPosition, vMeshIndices.data(), nMeshIndexSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setDriftIndirect()
    {
        std::vector<DrawElementsIndirectCommand> vDriftIndirect(m_pImpl->nDepthCount - 1);
        for (int i = 0; i < m_pImpl->nDepthCount - 1; ++i)
        {
            vDriftIndirect[i].count = (GLuint)2;
            vDriftIndirect[i].primCount = 1;
            vDriftIndirect[i].firstIndex = 0;
            vDriftIndirect[i].baseVertex = i * m_pImpl->nCurveCount;
            vDriftIndirect[i].baseInstance = 0;
        }

        BufferBounder<IndirectBuffer> indirectMeshBounder(m_pDriftIndirect);

        int nDriftIndirectSize = int(vDriftIndirect.size() * sizeof(DrawElementsIndirectCommand));

        if (!m_pDriftIndirect->bookSpace(nDriftIndirectSize))
            return false;

        {
            BufferMounter<IndirectBuffer> indirectMounter(m_pDriftIndirect);

            if (void* pPosition = indirectMounter.get_buffer())
                memcpy(pPosition, vDriftIndirect.data(), nDriftIndirectSize);
            else
                return false;
        }

        return true;
    }

    bool RenderBoreSurface::setDriftIndex()
    {
        std::vector<unsigned int> vDriftIndices(2);

        vDriftIndices[0] = (GLuint)m_pImpl->nCurveCount;
        vDriftIndices[1] = 0;

        BufferBounder<IndexBuffer> indexMeshBounder(m_pDriftIndex);

        int nDriftIndexSize = (int)vDriftIndices.size() * sizeof(unsigned int);

        if (!m_pDriftIndex->bookSpace(nDriftIndexSize))
            return false;

        {
            BufferMounter<IndexBuffer> indexMeshMounter(m_pDriftIndex);

            if (void* pPosition = indexMeshMounter.get_buffer())
                memcpy(pPosition, vDriftIndices.data(), nDriftIndexSize);
            else
                return false;
        }

        return true;
    }

	void RenderBoreSurface::draw()
	{
        if (!(m_bDataInit && m_bMiaInit && m_bPaletteInit && m_bProgramInit))
            return;

        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //---------------------------------------------------------------------------

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);
        BufferBounder<RenderBoreSurface> renderBounder(this);

        BufferBounder<TextureBuffer> paletteBounder(m_pPaletteBuffer);
        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);
        BufferBounder<ShaderStorageBuffer> depthBounder(m_pBufferDepth);
        BufferBounder<IndirectBuffer> indirectSurfaceBounder(m_pSurfaceIndirect);
        BufferBounder<IndexBuffer> indexSurfaceBounder(m_pSurfaceIndex);

        glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nDepthCount - 1,
            0);

        if (!m_bDrawMesh)
            return;

        //---------------------------------------------------------------------------
    
        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);

        BufferBounder<VertexBuffer>         vertexMeshBounder(m_VertexBuffer);
        BufferBounder<ShaderStorageBuffer>  depthMeshBounder(m_pBufferDepth);

        //---------------------------------------------------------------------------

        BufferBounder<IndirectBuffer>   indirectDriftBounder(m_pDriftIndirect);
        BufferBounder<IndexBuffer>      indexDriftBounder(m_pDriftIndex);

        m_pMeshProgram->setUniformVecf("m_vMesColor", &m_vZeroLineColor[0]);

        glLineWidth(m_fZeroLineWidth);

        glMultiDrawElementsIndirect(GL_LINES,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nDepthCount - 1,
            0);

        //---------------------------------------------------------------------------

        BufferBounder<IndirectBuffer>       indirectMeshBounder(m_pMeshIndirect);
        BufferBounder<IndexBuffer>          indexMeshBounder(m_pMeshIndex);

        m_pMeshProgram->setUniformVecf("m_vMesColor", &m_vMeshColor[0]);

        glLineWidth(1);

        glMultiDrawElementsIndirect(GL_LINES,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nDepthCount - 1,
            0);
    }

    bool RenderBoreSurface::InitBore3D(IBoreData* pData_, float fLogPerPixel_)
    {
        m_pImpl->pData = pData_;

        m_pImpl->nCurveCount = m_pImpl->pData->GetCurveCount();

        if (m_pImpl->nCurveCount > 0)
            m_pImpl->nDepthCount = (int)m_pImpl->pData->GetDepths().size();
        else
            return false;

        m_pImpl->bIsDiameters = m_pImpl->pData->IsDiameters();

        //----------------------------------------------------------------------------------

        m_pImpl->vDepths.resize(m_pImpl->nDepthCount);
        m_pImpl->vRotation.resize(m_pImpl->nDepthCount);

        m_pImpl->vRadiusCurve.resize(m_pImpl->nCurveCount * m_pImpl->nDepthCount);

        //----------------------------------------------------------------------------------

        for (int i = 0; i < m_pImpl->nDepthCount; ++i)
        {
            if (m_pImpl->pData->GetRotation().empty())
                m_pImpl->vRotation[i] = 0.f;
            else
                m_pImpl->vRotation[i] = (float)(m_pImpl->pData->GetRotation().data()[i]);

            for (int j = 0; j < m_pImpl->nCurveCount; ++j)
            {
                if (m_pImpl->pData->IsDiameters())
                    m_pImpl->vRadiusCurve[i * m_pImpl->nCurveCount + j] = (float)(m_pImpl->pData->GetRadiusCurve(i).data()[j]) / 2;
                else
                    m_pImpl->vRadiusCurve[i * m_pImpl->nCurveCount + j] = (float)(m_pImpl->pData->GetRadiusCurve(i).data()[j]);

            }
        }

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        if (!setVertexBuffer())
            return false;

        m_pSufraceProgram->setUniform1i("m_nCurveCount", &(m_pImpl->nCurveCount));

        //----------------------------------------------------------------------------------

        if (!setSurfaceIndex())
            return false;

        if (!setSurfaceIndirect())
            return false;

        if (!setMeshIndex())
            return false;

        if (!setMeshIndirect())
            return false;

        if (!setDriftIndex())
            return false;

        if (!setDriftIndirect())
            return false;

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);

        m_pMeshProgram->setUniform1i("m_nCurveCount", &(m_pImpl->nCurveCount));

        //----------------------------------------------------------------------------------

        m_bDataInit = true;

        return true;
    }

    bool RenderBoreSurface::InitPalette(const std::vector<COLORREF>& vecPalette_)
    {
        if (vecPalette_.size() < 1)
            return false;

        const COLORREF rgbRed   = 0x000000FF;
        const COLORREF rgbGreen = 0x0000FF00;
        const COLORREF rgbBlue  = 0x00FF0000;

        std::vector<float[3]> vPalette(vecPalette_.size());
        for (size_t i = 0; i < vPalette.size(); ++i)
        {
            auto a = (vecPalette_[i] & rgbGreen);
            auto b = (vecPalette_[i] & rgbGreen) >> 8;
            auto c = (float)((vecPalette_[i] & rgbGreen) >> 8) / 255;

            vPalette[i][0] = (float)((vecPalette_[i] & rgbRed)       >> 0 ) / 255;
            vPalette[i][1] = (float)((vecPalette_[i] & rgbGreen)     >> 8 ) / 255;
            vPalette[i][2] = (float)((vecPalette_[i] & rgbBlue)      >> 16) / 255;
        }

        float fPalleteLimit = 1.0f / vPalette.size() / 2;

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);

        m_pSufraceProgram->setUniform1f("m_fPalleteLimit", &fPalleteLimit);

        //----------------------------------------------------------------------------------

        BufferBounder<TextureBuffer> paletteBounder(m_pPaletteBuffer);

        if (!m_pPaletteBuffer->fillBuffer1D(GL_RGB, vPalette.size(), GL_RGB, GL_FLOAT, vPalette.data()))
            return false;

        m_bPaletteInit = true;

        return true;
    }

    bool RenderBoreSurface::InitDiaMapper(IDiaMapper* pMapper_)
    {
        m_fDepthMin = std::numeric_limits<float>::max();
        m_fDepthMax = -std::numeric_limits<float>::max();

        m_pImpl->pMapper = pMapper_;

        for (int i = 0; i < m_pImpl->nDepthCount; ++i)
        {
            m_pImpl->vDepths[i] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetDepths().data()[i]);

            m_fDepthMin = std::min(m_fDepthMin, m_pImpl->vDepths[i]);
            m_fDepthMax = std::max(m_fDepthMax, m_pImpl->vDepths[i]);
        }

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);

        //----------------------------------------------------------------------------------

        if (!setBufferDepth())
            return false;

        if (!setBufferAngle())
            return false;

        //----------------------------------------------------------------------------------

        m_bMiaInit = true;

        return true;
    }


    int RenderBoreSurface::GetBitmap(const RECT* pVisualRect, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        m_bDrawMesh = bDrawMesh;

        float fHalfWidth = float(pVisualRect->right- pVisualRect->left)*0.5f;

        m_mPRV = glm::ortho(
            -fHalfWidth, fHalfWidth,
            (float)pVisualRect->top, (float)pVisualRect->bottom,
            -fHalfWidth, fHalfWidth
        );

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);

        m_pSufraceProgram->setUniform1f("m_fRotation", &fRotation);
        m_pSufraceProgram->setUniform1f("m_fMinRadius", &fMinRadius);
        m_pSufraceProgram->setUniform1f("m_fMaxRadius", &fMaxRadius);
        m_pSufraceProgram->setUniform1i("m_nMinRadiusLP", &nMinRadiusLP);
        m_pSufraceProgram->setUniform1i("m_nMaxRadiusLP", &nMaxRadiusLP);
        m_pSufraceProgram->setUniform1f("m_fIsometryAngle", &fIsometryAngle);

        m_pSufraceProgram->setUniform1f("m_fPaletteValueMin", &fMinRadius);
        m_pSufraceProgram->setUniform1f("m_fPaletteValueMax", &fMaxRadius);

        m_pSufraceProgram->setUniformMat4f("m_MVP", &m_mPRV[0][0]);

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);

        m_pMeshProgram->setUniform1f("m_fRotation", &fRotation);
        m_pMeshProgram->setUniform1f("m_fMinRadius", &fMinRadius);
        m_pMeshProgram->setUniform1f("m_fMaxRadius", &fMaxRadius);
        m_pMeshProgram->setUniform1i("m_nMinRadiusLP", &nMinRadiusLP);
        m_pMeshProgram->setUniform1i("m_nMaxRadiusLP", &nMaxRadiusLP);
        m_pMeshProgram->setUniform1f("m_fIsometryAngle", &fIsometryAngle);

        m_pMeshProgram->setUniformMat4f("m_MVP", &m_mPRV[0][0]);

        //----------------------------------------------------------------------------------

        return 0;
    }

    void RenderBoreSurface::setBkgColor(float r_, float g_, float b_)
    {
        m_vBkgColor[0] = r_;
        m_vBkgColor[1] = g_;
        m_vBkgColor[2] = b_;
    }

    void RenderBoreSurface::setMesColor(float r_, float g_, float b_)
    {
        m_vMeshColor[0] = r_;
        m_vMeshColor[1] = g_;
        m_vMeshColor[2] = b_;
    }

    void RenderBoreSurface::setZeroLineColor(float r_, float g_, float b_, int nWidth_)
    {
        m_fZeroLineWidth = (float)nWidth_;
        m_vZeroLineColor[0] = r_;
        m_vZeroLineColor[1] = g_;
        m_vZeroLineColor[2] = b_;
    }

    bool RenderBoreSurface::init()
    {
        int nVersionSupported = getVersionGl();

        const GLubyte* pVersion = glGetString(GL_VERSION);

        if (!setSufraceProgram())
            return false;

        glGenVertexArrays(1, &m_nVAO);

        if (!setMeshProgram())
            return false;

        //----------------------------------------------------------------------------------

        m_VertexBuffer      = VertexBuffer::Create();
        m_pSurfaceIndirect  = IndirectBuffer::Create();
        m_pSurfaceIndex     = IndexBuffer::Create();
        m_pBufferDepth      = ShaderStorageBuffer::Create(0);
        m_pBufferAngle      = ShaderStorageBuffer::Create(1);

        m_pPaletteBuffer    = TextureBuffer::Create(GL_TEXTURE_1D, GL_TEXTURE0, GL_LINEAR);
        m_pPaletteBuffer->alignment(1);

        m_pMeshIndirect     = IndirectBuffer::Create();
        m_pMeshIndex        = IndexBuffer::Create();

        m_pDriftIndirect    = IndirectBuffer::Create();
        m_pDriftIndex       = IndexBuffer::Create();

        m_bProgramInit = true;

        return true;
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