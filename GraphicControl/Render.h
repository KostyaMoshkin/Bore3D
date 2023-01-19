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

         static const int mesh_fragment     = IDR_TEXTFILE3;
         static const int mesh_vertex       = IDR_TEXTFILE4;

         static const char* getName(int ID_)
         {
             switch (ID_)
             {
             case bore_vertex:      return "bore_vertex";
             case bore_fragment:    return "bore_fragment";
             case mesh_vertex:      return "bore_vertex";
             case mesh_fragment:    return "bore_fragment";
             default: return "";
             }
         }
     };

      class Render;
      using RenderPtr = std::shared_ptr<Render>;

      // Класс интерфейс рисования. Для каждой версии OpenGL и для каждой графической сущности свой наследник
      class Render// : public lib::CConfig
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
            virtual void lookAt(Matrix4& mView_) = 0;
            virtual void rotate(Matrix4& mRotate_) = 0;
            virtual void translate(Matrix4& mTranslate_) = 0;
            virtual void setViewAngle(Matrix4& mPerspective_) = 0;
            virtual void draw() = 0;

            virtual void bound() = 0;
            virtual void unbound() = 0;

      public:
          void setVisible(bool bVisible_) { m_bVisible = bVisible_; }
          bool isVisible() { return m_bVisible; }

          void setVersionGl(int nVersionFull_) { m_nVersionFull = nVersionFull_; }
          int getVersionGl() {
                  static int nVersionFull = -1;

                  if (nVersionFull != -1)
                      return nVersionFull;

                  //------------------------------------------------------------------------------------------

                  const GLubyte* pVersion = glGetString(GL_VERSION);
                  if (!pVersion)
                      return nVersionFull;

                  std::string sVersion = (const char*)pVersion;
                  if (sVersion.length() < 3)
                      return nVersionFull;

                  int nVersionMain = sVersion[0] - '0';
                  int nVersionSub = sVersion[2] - '0';

                  nVersionFull = nVersionMain * 10 + nVersionSub;
                  //       nVersionFull = 30;

                        //------------------------------------------------------------------------------------------

                  //if ( nVersionFull < 20 )
                  //  FORCE_ASSERTM("Current Open GL version is not supported.");

                  return nVersionFull;
          }

          virtual void sizeChanged(int nWidth_, int nHeight_) {; }

      };

}
