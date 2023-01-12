#pragma once
#include "pch.h"

#include "Render.h"

#include <array>

namespace GL
{
    class RenderBoreSurface;
    using RenderBoreSurfacePtr = std::shared_ptr<RenderBoreSurface>;

    class RenderBoreSurface : public Render
    {
        std::array<float, 3> m_vBkgColor;

    public:
        RenderBoreSurface();

    public:
        // Унаследовано через Render
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

    protected:
        void setBkgColor(float r_, float g_, float b_);

    public:
        static RenderBoreSurfacePtr Create() { return std::make_shared<RenderBoreSurface>(); }

    };
}
