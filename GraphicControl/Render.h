#pragma once
#include "pch.h"

#include "resource.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <memory>

 namespace GL {

     using Matrix4 = glm::mat4x4;
     using Vector3 = glm::vec3;
     using Vector4 = glm::vec4;

     using dPoint3D = glm::f32vec3;
     using iPoint3D = glm::i32vec3;


     struct ShaderName
     {
         static const int bore_fragment     = IDR_TEXTFILE1;
         static const int bore_vertex       = IDR_TEXTFILE2;
         static const int bore_geometry     = IDR_TEXTFILE5;

         static const int mesh_fragment     = IDR_TEXTFILE3;
         static const int mesh_vertex       = IDR_TEXTFILE4;
         static const int mesh_geometry     = IDR_TEXTFILE6;

         static const char* getName(int ID_)
         {
             switch (ID_)
             {
             case bore_vertex:      return "bore_vertex";
             case bore_fragment:    return "bore_fragment";
             case bore_geometry:    return "bore_geometry";
             case mesh_vertex:      return "mesh_vertex";
             case mesh_fragment:    return "mesh_fragment";
             case mesh_geometry:    return "mesh_geometry";
             default: return "";
             }
         }
     };

      class Render;
      using RenderPtr = std::shared_ptr<Render>;

      // Класс интерфейс рисования. Для каждой версии OpenGL и для каждой графической сущности свой наследник
      class Render
      {
          bool m_bVisible = false;
          int m_nVersionFull = 0;

      protected:
          float m_fScale = 5.0f;

      public:
          Render() = default;
          virtual ~Render() = default;

        public:
            virtual bool init() = 0;
            virtual void lookAt(Matrix4& mView_) {}
            virtual void rotate(Matrix4& mRotate_) {}
            virtual void translate(Matrix4& mTranslate_) {}
            virtual void setViewAngle(Matrix4& mPerspective_) {}
            virtual void draw() = 0;

            virtual void bound() = 0;
            virtual void unbound() = 0;

            virtual std::string getErrorMessage() = 0;

      public:
          void setVisible(bool bVisible_) { m_bVisible = bVisible_; }
          bool isVisible() { return m_bVisible; }

          void setVersionGl(int nVersionFull_) { m_nVersionFull = nVersionFull_; }
          int getVersionGl();

          virtual void sizeChanged(int nWidth_, int nHeight_) {; }

      };

}
