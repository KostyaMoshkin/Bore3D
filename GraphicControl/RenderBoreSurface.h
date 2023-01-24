#pragma once
#include "pch.h"

#include "Render.h"

#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"
#include "IndexBuffer.h"
#include "IndirectBuffer.h"
#include "ShaderStorageBuffer.h"

#include <array>

namespace GL
{
    class RenderBoreSurface;
    using RenderBoreSurfacePtr = std::shared_ptr<RenderBoreSurface>;

    class RenderBoreSurface : public Render
    {

        typedef  struct
        {
            GLuint  count;
            GLuint  primCount;
            GLuint  firstIndex;
            GLint   baseVertex;
            GLuint  baseInstance;
        } DrawElementsIndirectCommand;

        std::array<float, 3> m_vBkgColor = { 1, 1, 1 };
        std::array<float, 3> m_vMeshColor = { 0, 0, 0 };
        std::array<float, 3> m_vZeroLineColor = { 1, 1, 1 };
        float m_fZeroLineWidth = 3;

        unsigned int m_nVAO;

        float m_fDepthMin;
        float m_fDepthMax;

        bool m_bDrawMesh = false;

        Matrix4 m_mPRV;

        ShaderProgramPtr m_pSufraceProgram        = nullptr;
        VertexBufferPtr m_VertexBuffer            = nullptr;
        TextureBufferPtr m_pPaletteBuffer         = nullptr;
        IndirectBufferPtr m_pSurfaceIndirect      = nullptr;
        IndexBufferPtr m_pSurfaceIndex            = nullptr;
        ShaderStorageBufferPtr m_pBufferDepth     = nullptr;
        ShaderStorageBufferPtr m_pBufferAngle     = nullptr;

        ShaderProgramPtr    m_pMeshProgram        = nullptr;
        IndirectBufferPtr   m_pMeshIndirect       = nullptr;
        IndexBufferPtr      m_pMeshIndex          = nullptr;

        IndirectBufferPtr   m_pDriftIndirect       = nullptr;
        IndexBufferPtr      m_pDriftIndex          = nullptr;

        bool m_bDataInit = false;
        bool m_bMiaInit = false;
        bool m_bPaletteInit = false;
        bool m_bProgramInit = false;

    public:
        RenderBoreSurface();

    private:
        bool setSufraceProgram();
        bool setVertexBuffer();
        bool setSurfaceIndirect();
        bool setSurfaceIndex();
        bool setBufferDepth();
        bool setBufferAngle();

        bool setMeshProgram();
        bool setMeshIndirect();
        bool setMeshIndex();

        bool setDriftIndirect();
        bool setDriftIndex();

    public:
        // ������������ ����� Render
        bool init() override;
        void draw() override;
        void bound() override;
        void unbound() override;

    public:
        // �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
        bool InitBore3D(
            void* pData_, // ��������� ������� � ������ ��������
            float fLogPerPixel_ // ����������� ����������� ����� ����������� ��������� (������������ ��������) � ��������� ������
        );

        bool InitPalette(const std::vector<COLORREF>& vecPalette_);

        bool InitDiaMapper(
            void* pMapper_  // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
        );

        int GetBitmap(
            const RECT* pVisualRect, // ������������� � ���������� �������� ������������ ����� 3D-������ (top,bottom ������������� fTop,fBottom ��� �������������� � pMapper)
            float fTop, float fBottom, // �������� ������ (����) ����������� ������ ��������
            float fRotation, // �������������� ���� �������� ����� ������ ������ ����� ���
            // ��������� ��������� 4 ��������� ���������� ����� ��� ����������� �������� (��� �������� ������� ������������� � ������� ������� ������)
            float fMinRadius, float fMaxRadius, // ����������� � ������������ �������� �������/��������, ��������������� ������ 
            int nMinRadiusLP, int nMaxRadiusLP, // ������� ������ � ���������� �������� ��� ������������ � ������������� ������� 
            float fIsometryAngle, // ���� �������������� ��������
            bool bDrawMesh
        );

    public:
        void setBkgColor(float r_, float g_, float b_);
        void setMesColor(float r_, float g_, float b_);
        void setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1);

    public:
        static RenderBoreSurfacePtr Create() { return std::make_shared<RenderBoreSurface>(); }

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m_pImpl;
    };
}
