#pragma once
#include "pch.h"

#include "Render.h"

#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"
#include "IndexBuffer.h"
#include "IndirectBuffer.h"
#include "ShaderStorageBuffer.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

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

        int m_nInterpolateCount = 1;

        // Вектор индексов и соответствующих интервалов глубин
        std::vector<std::tuple<int, float, float>> m_vDepthSercher;

        // Индексы отображения глубин
        int m_nStartDepthIndex = 0;
        int m_nStopDepthIndex = 0;

        //  Возрастания или убывание значений глубины
        bool m_bDepthIncreasing = true;

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

        void calcViewIndices(const RECT* , float, int );

    public:
        // Унаследовано через Render
        bool init() override;
        void draw() override;
        void bound() override;
        void unbound() override;

    public:
        // прототип метода для отображения части 3D-ствола скважины в bitmap
        bool InitBore3D(
            IBoreData* pData_, // интерфейс доступа к данным развёртки
            int nMeshStep_ // шаг между линиями горизонтальной сетки
        );

        bool InitPalette(const std::vector<COLORREF>& vecPalette_);

        bool InitDiaMapper(
            IDiaMapper* pMapper_  // отображение глубина <--> логические единицы по вертикали (не путать с пикселями)
        );

        int GetBitmap(
            const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
            float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
            // совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
            float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
            int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
            float fIsometryAngle, // угол изометрической проекции
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
