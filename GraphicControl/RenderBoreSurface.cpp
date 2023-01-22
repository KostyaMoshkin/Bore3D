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
        setBkgColor(1, 1, 1);

        std::srand((unsigned)std::time(nullptr));

        m_pImpl = std::make_shared<Implementation>();
    }
	
	void RenderBoreSurface::draw()
	{
        if (!(m_bDataInit && m_bMiaInit && m_bPaletteInit && m_bProgramInit))
            return;


        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //---------------------------------------------------------------------------

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        BufferBounder<TextureBuffer> paletteBounder(m_pPaletteBuffer);
        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);
        BufferBounder<ShaderStorageBuffer> depthBounder(m_pBufferDepth);
        BufferBounder<IndirectBuffer> indirectSurfaceBounder(m_pSurfaceIndirect);
        BufferBounder<IndexBuffer> indexSurfaceBounder(m_pSurfaceIndex);

        glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nCurveCount - 1,
            0);

        renderBoreBounder.unbound();

        if (!m_bDrawMesh)
            return;

        //---------------------------------------------------------------------------
    
        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);
        BufferBounder<RenderBoreSurface> renderMeshBoreBounder(this);

        BufferBounder<VertexBuffer> vertexMeshBounder(m_VertexBuffer);
        BufferBounder<ShaderStorageBuffer> depthMeshBounder(m_pBufferDepth);
        BufferBounder<IndirectBuffer> indirectMeshBounder(m_pMeshIndirect);
        BufferBounder<IndexBuffer> indexMeshBounder(m_pMeshIndex);

        glLineWidth(1);

        glMultiDrawElementsIndirect(GL_LINES,
            GL_UNSIGNED_INT,
            nullptr,
            m_pImpl->nCurveCount - 1,
            0);

        renderMeshBoreBounder.unbound();
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

        float fRadiusMin = std::numeric_limits<float>::max();
        float fRadiusMax = -std::numeric_limits<float>::max();

        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
        {
            m_pImpl->vRotation[i] = (float)(m_pImpl->pData->GetRotation().data()[i]);

            for (int j = 0; j < m_pImpl->nDriftCount; ++j)
            {
                m_pImpl->vRadiusCurve[i * m_pImpl->nDriftCount + j] = (float)(m_pImpl->pData->GetRadiusCurve(i).data()[j]);
                fRadiusMin = std::min(fRadiusMin, m_pImpl->vRadiusCurve[i * m_pImpl->nDriftCount + j]);
                fRadiusMax = std::max(fRadiusMax, m_pImpl->vRadiusCurve[i * m_pImpl->nDriftCount + j]);
            }
        }

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        m_pSufraceProgram->setUniform1f("m_fPaletteValueMin", &fRadiusMin);
        m_pSufraceProgram->setUniform1f("m_fPaletteValueMax", &fRadiusMax);

        //----------------------------------------------------------------------------------

        BufferBounder<VertexBuffer> vertexBounder(m_VertexBuffer);

        int nVertexBufferSize = int(m_pImpl->nCurveCount * m_pImpl->nDriftCount * sizeof(float));

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

        m_pSufraceProgram->setUniform1i("m_nCurveCount", &(m_pImpl->nCurveCount));
        m_pSufraceProgram->setUniform1i("m_nDriftCount", &(m_pImpl->nDriftCount));

        //----------------------------------------------------------------------------------

        std::vector<unsigned int> vSurfaceIndices(m_pImpl->nDriftCount * 2 + 2);  // +2 т к надо замкнуть окружность, последн€€ точка соедин€етс€ с первой

        for (unsigned int i = 0; i < (unsigned int)m_pImpl->nDriftCount; ++i)
        {
            vSurfaceIndices[i * 2] = m_pImpl->nDriftCount + i;
            vSurfaceIndices[i * 2 + 1] = i;
        }

        //  Ќеобходимо домолнить индексы, чтобы последн€€ точка в р€ду замкнулась с первой
        vSurfaceIndices[m_pImpl->nDriftCount * 2 + 0] = m_pImpl->nDriftCount;
        vSurfaceIndices[m_pImpl->nDriftCount * 2 + 1] = 0;

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

        //----------------------------------------------------------------------------------

        std::vector<DrawElementsIndirectCommand> vSurfaceIndirect(m_pImpl->nCurveCount);
        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
        {
            vSurfaceIndirect[i].count = (GLuint)vSurfaceIndices.size();
            vSurfaceIndirect[i].primCount = 1;
            vSurfaceIndirect[i].firstIndex = 0;
            vSurfaceIndirect[i].baseVertex = i * m_pImpl->nDriftCount;
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

        //----------------------------------------------------------------------------------

        renderBoreBounder.unbound();

        //----------------------------------------------------------------------------------

        std::vector<unsigned int> vMeshIndices(m_pImpl->nDriftCount * 4);

        for (unsigned int i = 0; i < (unsigned int)m_pImpl->nDriftCount; ++i)
        {
            vMeshIndices[i * 4 + 0] = (GLuint)m_pImpl->nDriftCount + i;
            vMeshIndices[i * 4 + 1] = i;
            vMeshIndices[i * 4 + 2] = i;
            vMeshIndices[i * 4 + 3] = i + 1;
        }

        //  Ќеобходимо домолнить индексы, чтобы последн€€ точка в р€ду замкнулась с первой
        vMeshIndices[(m_pImpl->nDriftCount - 1) * 4 + 3] = 0;

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

        //----------------------------------------------------------------------------------

        std::vector<DrawElementsIndirectCommand> vMeshIndirect(m_pImpl->nCurveCount - 1);
        for (int i = 0; i < m_pImpl->nCurveCount - 1; ++i)
        {
            vMeshIndirect[i].count = (GLuint)vMeshIndices.size();
            vMeshIndirect[i].primCount = 1;
            vMeshIndirect[i].firstIndex = 0;
            vMeshIndirect[i].baseVertex = i * m_pImpl->nDriftCount;
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

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);
        BufferBounder<RenderBoreSurface> renderMeshBoreBounder(this);

        m_pMeshProgram->setUniform1i("m_nCurveCount", &(m_pImpl->nCurveCount));
        m_pMeshProgram->setUniform1i("m_nDriftCount", &(m_pImpl->nDriftCount));

        renderMeshBoreBounder.unbound();

        //----------------------------------------------------------------------------------

        m_bDataInit = true;

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
        m_fDepthMin = std::numeric_limits<float>::max();
        m_fDepthMax = -std::numeric_limits<float>::max();

        m_pImpl->pMapper = (DataProvider::IDiaMapper *)pMapper_;

        for (int i = 0; i < m_pImpl->nCurveCount; ++i)
        {
            m_pImpl->vDepths[i] = (float)m_pImpl->pMapper->GeoToLP(m_pImpl->pData->GetDepths().data()[i]);
            m_fDepthMin = std::min(m_fDepthMin, m_pImpl->vDepths[i]);
            m_fDepthMax = std::max(m_fDepthMax, m_pImpl->vDepths[i]);
        }

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

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

        BufferBounder<ShaderStorageBuffer> angleBounder(m_pBufferAngle);

        int nBufferAngleSize = int(m_pImpl->nCurveCount * sizeof(float));

        if (!m_pBufferAngle->bookSpace(nBufferAngleSize))
            return false;

        {
            BufferMounter<ShaderStorageBuffer> angleMounter(m_pBufferAngle);

            if (void* pPosition = angleMounter.get_buffer())
                memcpy(pPosition, m_pImpl->vRotation.data(), nBufferAngleSize);
            else
                return false;
        }

        //----------------------------------------------------------------------------------

        renderBoreBounder.unbound();

        //----------------------------------------------------------------------------------

        m_bMiaInit = true;

        return true;
    }


    int RenderBoreSurface::GetBitmap(const RECT* pVisualRect, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
    {
        m_bDrawMesh = bDrawMesh;

        m_mPRV = glm::ortho(-fMaxRadius, fMaxRadius, m_fDepthMin, m_fDepthMax, -fMaxRadius, fMaxRadius);

        BufferBounder<ShaderProgram> surfaceBounder(m_pSufraceProgram);
        BufferBounder<RenderBoreSurface> renderBoreBounder(this);

        m_pSufraceProgram->setUniform1f("m_fTop", &fTop);
        m_pSufraceProgram->setUniform1f("m_fBottom", &fBottom);
        m_pSufraceProgram->setUniform1f("m_fRotation", &fRotation);
        m_pSufraceProgram->setUniform1f("m_fMinRadius", &fMinRadius);
        m_pSufraceProgram->setUniform1f("m_fMaxRadius", &fMaxRadius);
        m_pSufraceProgram->setUniform1i("m_nMinRadiusLP", &nMinRadiusLP);
        m_pSufraceProgram->setUniform1i("m_nMaxRadiusLP", &nMaxRadiusLP);
        m_pSufraceProgram->setUniform1f("m_fIsometryAngle", &fIsometryAngle);

        m_pSufraceProgram->setUniformMat4f("m_MVP", &m_mPRV[0][0]);

        renderBoreBounder.unbound();

        //----------------------------------------------------------------------------------

        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);
        BufferBounder<RenderBoreSurface> renderMeshBoreBounder(this);

        m_pMeshProgram->setUniform1f("m_fTop", &fTop);
        m_pMeshProgram->setUniform1f("m_fBottom", &fBottom);
        m_pMeshProgram->setUniform1f("m_fRotation", &fRotation);
        m_pMeshProgram->setUniform1f("m_fMinRadius", &fMinRadius);
        m_pMeshProgram->setUniform1f("m_fMaxRadius", &fMaxRadius);
        m_pMeshProgram->setUniform1i("m_nMinRadiusLP", &nMinRadiusLP);
        m_pMeshProgram->setUniform1i("m_nMaxRadiusLP", &nMaxRadiusLP);
        m_pMeshProgram->setUniform1f("m_fIsometryAngle", &fIsometryAngle);

        m_pMeshProgram->setUniformMat4f("m_MVP", &m_mPRV[0][0]);

        renderMeshBoreBounder.unbound();

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
        std::array<float, 3> vMeshColor;

        vMeshColor[0] = r_;
        vMeshColor[1] = g_;
        vMeshColor[2] = b_;

        BufferBounder<ShaderProgram> meshBounder(m_pMeshProgram);
        BufferBounder<RenderBoreSurface> renderMeshBoreBounder(this);

        m_pMeshProgram->setUniformVecf("m_vMesColor", &vMeshColor[0]);

        renderMeshBoreBounder.unbound();
    }

    bool RenderBoreSurface::init()
    {
        int nVersionSupported = getVersionGl();

        const GLubyte* pVersion = glGetString(GL_VERSION);

        ShaderProgramPtr pSufraceProgram = ShaderProgram::Create();

        bool bAddSufraceShaderError = false;

        bAddSufraceShaderError |= !pSufraceProgram->addShader(ShaderName::bore_fragment, ShaderProgram::ShaderType::Fragment());
        bAddSufraceShaderError |= !pSufraceProgram->addShader(ShaderName::bore_vertex, ShaderProgram::ShaderType::Vertex());

        if (bAddSufraceShaderError)
            return false;

        if (!pSufraceProgram->init())
            return false;

        glGenVertexArrays(1, &m_nVAO);

        m_pSufraceProgram = pSufraceProgram;

        //----------------------------------------------------------------------------------

        ShaderProgramPtr pMeshProgram = ShaderProgram::Create();

        bool bAddMeshShaderError = false;

        bAddMeshShaderError |= !pMeshProgram->addShader(ShaderName::mesh_fragment, ShaderProgram::ShaderType::Fragment());
        bAddMeshShaderError |= !pMeshProgram->addShader(ShaderName::mesh_vertex, ShaderProgram::ShaderType::Vertex());

        if (bAddMeshShaderError)
            return false;

        if (!pMeshProgram->init())
            return false;

        glGenVertexArrays(1, &m_nVAO);

        m_pMeshProgram = pMeshProgram;

        //----------------------------------------------------------------------------------

        m_VertexBuffer      = VertexBuffer::Create();
        m_pSurfaceIndirect  = IndirectBuffer::Create();
        m_pMeshIndirect     = IndirectBuffer::Create();
        m_pSurfaceIndex     = IndexBuffer::Create();
        m_pMeshIndex        = IndexBuffer::Create();
        m_pBufferDepth      = ShaderStorageBuffer::Create(0);
        m_pBufferAngle      = ShaderStorageBuffer::Create(1);

        m_pPaletteBuffer    = TextureBuffer::Create(GL_TEXTURE_1D, GL_TEXTURE0, GL_LINEAR);
        m_pPaletteBuffer->alignment(1);

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