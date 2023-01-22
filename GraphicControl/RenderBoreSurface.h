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

        float m_fDepthMin;
        float m_fDepthMax;

        bool m_bDrawMesh = false;

        Matrix4 m_mPRV;

        ShaderProgramPtr m_pSufraceProgram  = nullptr;
        ShaderProgramPtr m_pMeshProgram     = nullptr;
        VertexBufferPtr m_VertexBuffer      = nullptr;
        TextureBufferPtr m_pPaletteBuffer   = nullptr;
        IndirectBufferPtr m_pSurfaceIndirect = nullptr;
        IndirectBufferPtr m_pMeshIndirect   = nullptr;
        IndexBufferPtr m_pSurfaceIndex      = nullptr;
        IndexBufferPtr m_pMeshIndex         = nullptr;
        ShaderStorageBufferPtr m_pBufferDepth     = nullptr;
        ShaderStorageBufferPtr m_pBufferAngle     = nullptr;

        bool m_bDataInit = false;
        bool m_bMiaInit = false;
        bool m_bPaletteInit = false;
        bool m_bProgramInit = false;

    public:
        RenderBoreSurface();

    public:
        // Унаследовано через Render
        bool init() override;
        void draw() override;
        void bound() override;
        void unbound() override;

    public:
        // прототип метода для отображения части 3D-ствола скважины в bitmap
        bool InitBore3D(
            void* pData_, // интерфейс доступа к данным развёртки
            float fLogPerPixel_ // коэффициент соотношения между логическими единицами (используются маппером) и пикселями экрана
        );

        bool InitPalette(const std::vector<COLORREF>& vecPalette_);

        bool InitDiaMapper(
            void* pMapper_  // отображение глубина <--> логические единицы по вертикали (не путать с пикселями)
        );

        int GetBitmap(
            const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
            float fTop, float fBottom, // интервал глубин (окно) отображения ствола скважины
            float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
            // совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
            float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
            int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
            float fIsometryAngle, // угол изометрической проекции
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
