#pragma once
#include "pch.h"

#include "Render.h"

#include "BufferOpenGL.h"
#include "TextureBuffer.h"
#include "ShaderProgram.h"

#include <array>

namespace GL
{
    class RenderBoreSurface;
    using RenderBoreSurfacePtr = std::shared_ptr<RenderBoreSurface>;

    class RenderBoreSurface : public Render
    {
        std::array<float, 3> m_vBkgColor;

        unsigned int m_nVAO;

        ShaderProgramPtr m_pShaderProgram = nullptr;
        BufferOpenGLPtr m_VertexBuffer = nullptr;
        TextureBufferPtr m_pPaletteBuffer = nullptr;

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
        float getScale() override;
        void setScale(float fScale_) override;

    public:
        // �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
        bool InitBore3D(
            void* pData_, // ��������� ������� � ������ ��������
            float fLogPerPixel_ // ����������� ����������� ����� ����������� ��������� (������������ ��������) � ��������� ������
        );

        bool InitPalette(const std::vector<COLORREF>& vecPalette_);

        void InitDiaMapper(
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
