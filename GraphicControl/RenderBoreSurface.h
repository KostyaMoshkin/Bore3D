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

        std::array<float, 3> m_vBkgColor;

        unsigned int m_nVAO;

        Matrix4 m_mPRV;

        ShaderProgramPtr m_pShaderProgram   = nullptr;
        VertexBufferPtr m_VertexBuffer      = nullptr;
        TextureBufferPtr m_pPaletteBuffer   = nullptr;
        IndirectBufferPtr m_pBufferIndirect = nullptr;
        IndexBufferPtr   m_pBufferIndex     = nullptr;
        ShaderStorageBufferPtr   m_pBufferDepth     = nullptr;

        bool m_bDataInit = false;
        bool m_bPaletteInit = false;
        bool m_bProgramInit = false;

    public:
        RenderBoreSurface();

    public:
        // ������������ ����� Render
        bool init() override;
        void draw() override;
        void lookAt(Matrix4& mView_) override;
        void rotate(Matrix4& mRotate_) override;
        void translate(Matrix4& mTranslate_) override;
        void setViewAngle(Matrix4& mPerspective_) override;
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

    protected:
        void setBkgColor(float r_, float g_, float b_);

    public:
        static RenderBoreSurfacePtr Create() { return std::make_shared<RenderBoreSurface>(); }

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m_pImpl;
    };
}
