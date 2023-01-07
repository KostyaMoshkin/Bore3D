#include "pch.h"
#include "ControlGL.h"

#include <glew.h>
#include <wglew.h>
#include <gl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ControlGL

namespace GraphicControl
{

    ControlGL::ControlGL()
    {
        m_vBkgColor[0] = 0;
        m_vBkgColor[1] = 0;
        m_vBkgColor[2] = 0;

        RegisterWindowClass();
    }

    ControlGL::~ControlGL()
    {
    }

    bool ControlGL::init()
    {
        if (m_rendContext != nullptr)
            return true;

        if (!initWindow())
            return false;

        if (!initGlew())
            return false;

        if (!createContext())
            return false;

        return true;
    }

    void ControlGL::setBkgColor(float r_, float g_, float b_)
    {
        m_vBkgColor[0] = r_;
        m_vBkgColor[1] = g_;
        m_vBkgColor[2] = b_;
    }

    bool ControlGL::beginDraw(int width_, int height_) const
    {
        if (!makeCurrent())
            return false;

        if (width_ * height_ == 0)
            return true;

        glClearColor(m_vBkgColor[0], m_vBkgColor[1], m_vBkgColor[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, width_, height_);
        glDisable(GL_DEPTH_TEST);

        return true;
    }

    void ControlGL::endDraw() const
    {
        SwapBuffers(m_hDC);

        wglMakeCurrent(m_hDC, nullptr);
    }

    bool ControlGL::makeCurrent() const
    {
        if (!m_rendContext)
            return false;

        if (!wglMakeCurrent(m_hDC, m_rendContext))
        {
            return false;
        }

        return true;
    }

    bool ControlGL::initWindow()
    {
        m_cDC = GetDC();
        m_hDC = *m_cDC;

        if (m_hDC == nullptr)
            return false;

        PIXELFORMATDESCRIPTOR pfd = {
          sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
          1,                     // version number  
          PFD_DRAW_TO_WINDOW |   // support window  
          PFD_SUPPORT_OPENGL |   // support OpenGL  
          PFD_DOUBLEBUFFER,      // double buffered  
          PFD_TYPE_RGBA,         // RGBA type  
          24,                    // 24-bit color depth  
          0, 0, 0, 0, 0, 0,      // color bits ignored  
          0,                     // no alpha buffer  
          0,                     // shift bit ignored  
          0,                     // no accumulation buffer  
          0, 0, 0, 0,            // accum bits ignored  
          32,                    // 32-bit z-buffer      
          0,                     // no stencil buffer  
          0,                     // no auxiliary buffer  
          PFD_MAIN_PLANE,        // main layer  
          0,                     // reserved  
          0, 0, 0                // layer masks ignored  
        };

        int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        if (pixelFormat == 0)
        {
            return false;
        }

        if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
        {
            return false;
        }

        return true;
    }

    bool ControlGL::initGlew()
    {
        static bool glew_created = false;

        if (glew_created)
            return true;

        //------------------------------------------------------------------------------------------
        // init temp context

        HGLRC tempContext = wglCreateContext(m_hDC);
        if (tempContext == nullptr)
        {
            return false;
        }

        if (!wglMakeCurrent(m_hDC, tempContext))
        {
            wglDeleteContext(tempContext);

            return false;
        }

        //------------------------------------------------------------------------------------------

        GLenum glewInitResult = UINT_MAX;

        try
        {
            glewInitResult = glewInit();
            wglewInit();
        }
        catch (...)
        {
        }

        //------------------------------------------------------------------------------------------

        if (glewInitResult != GLEW_OK)
        {
            wglMakeCurrent(m_hDC, nullptr);

            return false;
        }

        wglMakeCurrent(m_hDC, nullptr);
        glew_created = true;
        return true;
    }

    bool ControlGL::createContext()
    {
        m_rendContext = wglCreateContext(m_hDC);

        return m_rendContext != nullptr;
    }


    // Register the window class if it has not already been registered.
    BOOL ControlGL::RegisterWindowClass()
    {
        WNDCLASS wndcls;
        HINSTANCE hInst = AfxGetInstanceHandle();

        if (!(::GetClassInfo(hInst, CONTROLGL_CLASSNAME, &wndcls)))
        {
            // otherwise we need to register a new class
            wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
            wndcls.lpfnWndProc = ::DefWindowProc;
            wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
            wndcls.hInstance = hInst;
            wndcls.hIcon = NULL;
            wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
            wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
            wndcls.lpszMenuName = NULL;
            wndcls.lpszClassName = CONTROLGL_CLASSNAME;

            if (!AfxRegisterClass(&wndcls))
            {
                AfxThrowResourceException();
                return FALSE;
            }
        }

        return TRUE;
    }


    BEGIN_MESSAGE_MAP(ControlGL, CWnd)
        //{{AFX_MSG_MAP(CBitmapViewer)
        ON_WM_PAINT()
        ON_WM_ERASEBKGND()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    /////////////////////////////////////////////////////////////////////////////
    // CBitmapViewer message handlers

    void ControlGL::OnPaint()
    {
        CRect rc;
        this->GetWindowRect(&rc);

        beginDraw(rc.Width(), rc.Height());
        paint();
        endDraw();
    }

    BOOL ControlGL::OnEraseBkgnd(CDC* pDC)
    {
        return CWnd::OnEraseBkgnd(pDC);
    }

    void ControlGL::PreSubclassWindow()
    {
        CWnd::PreSubclassWindow();
    }

    /////////////////////////////////////////////////////////////////////////////
    // CBitmapViewer methods

    BOOL ControlGL::Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle /*=WS_VISIBLE*/)
    {
        return CWnd::Create(CONTROLGL_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
    }
}