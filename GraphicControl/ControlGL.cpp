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
        m_ptWindow.SetPoint(0, 0);

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

    bool ControlGL::beginDraw(int width_, int height_) const
    {
        if (!makeCurrent())
            return false;

        //if (width_ * height_ == 0)
        //    return true;  //  Может FALSE

        glViewport(0, 0, width_, height_);
        glDisable(GL_DEPTH_TEST);

        return true;
    }

    void ControlGL::endDraw() const
    {
        SwapBuffers(m_GLHDC);

        wglMakeCurrent(m_GLHDC, nullptr);
    }

    bool ControlGL::makeCurrent() const
    {
        if (!m_rendContext)
            return false;

        if (!wglMakeCurrent(m_GLHDC, m_rendContext))
        {
            return false;
        }

        return true;
    }

    bool ControlGL::initWindow()
    {
        CDC* pDC = GetDC();
        m_GLHDC = *pDC;

        if (m_GLHDC == nullptr)
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

        int pixelFormat = ChoosePixelFormat(m_GLHDC, &pfd);
        if (pixelFormat == 0)
        {
            return false;
        }

        if (!SetPixelFormat(m_GLHDC, pixelFormat, &pfd))
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

        HGLRC tempContext = wglCreateContext(m_GLHDC);
        if (tempContext == nullptr)
        {
            return false;
        }

        if (!wglMakeCurrent(m_GLHDC, tempContext))
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
            wglMakeCurrent(m_GLHDC, nullptr);

            return false;
        }

        wglMakeCurrent(m_GLHDC, nullptr);
        glew_created = true;
        return true;
    }

    bool ControlGL::createContext()
    {
        m_rendContext = wglCreateContext(m_GLHDC);

        return m_rendContext != nullptr;
    }

    void ControlGL::SavePicture()
    {
        glReadPixels(0, 0, m_ptWindow.x, m_ptWindow.y, GL_BGRA, GL_UNSIGNED_BYTE, m_vPrintScreen.data());

        HBITMAP resultBitmap = CreateBitmap(m_ptWindow.x, m_ptWindow.y, 1, 32, (void*)m_vPrintScreen.data());

        HDC MemDC = CreateCompatibleDC(m_saveHDC);

        auto hOldBmp = SelectObject(MemDC, resultBitmap);

        //StretchBlt(m_saveHDC, 0, 0, m_ptWindow.x, m_ptWindow.y, MemDC, 0, 0, m_ptWindow.x, m_ptWindow.y, SRCCOPY);
        BitBlt(m_saveHDC, 0, 0, m_ptWindow.x, m_ptWindow.y, MemDC, 0, 0, SRCCOPY);

        SelectObject(MemDC, hOldBmp);

        DeleteDC(MemDC);

        DeleteObject(resultBitmap);
    }

    void ControlGL::fillPicture(HDC hDC_)
    {
        m_saveHDC = hDC_;

        OnPaint(true);
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

    void ControlGL::OnPaint(bool bSave_)
    {
        if (m_rendContext == nullptr)
            return;

        CRect rcWindow;
        this->GetWindowRect(&rcWindow);

        bool bSizeChanged = !((m_ptWindow.x == rcWindow.Width()) && (m_ptWindow.y == rcWindow.Height()));

        if (bSizeChanged)
            changeWindowSize(rcWindow);

        if (!(m_bNeedUpdate || bSizeChanged))
        {
            Sleep(1);
            return;
        }

        m_bNeedUpdate = false;

        if (rcWindow.IsRectEmpty())
            return;

        if (!beginDraw(rcWindow.Width(), rcWindow.Height()))
            return;

        paint();

        if (bSave_)
            SavePicture();

        endDraw();
    }

    void ControlGL::changeWindowSize(CRect rcWindow_)
    {
        m_ptWindow.SetPoint(rcWindow_.Width(), rcWindow_.Height());

        m_vPrintScreen.resize(m_ptWindow.x * m_ptWindow.y * 4);
    }

    void ControlGL::OnPaint()
    {
        OnPaint(false);
    }

    BOOL ControlGL::OnEraseBkgnd(CDC* pDC)
    {
        return CWnd::OnEraseBkgnd(pDC);
    }

    void ControlGL::OnSize(UINT nType, int cx, int cy)
    {
        m_ptWindow.SetPoint(cx, cy);
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

/*

    void CRenderView::SnapClient()
    {
        BeginWaitCursor();
     
       // Получаем геометрию клиента
       CRect rect;
       GetClientRect(&rect);
       CSize size(rect.Width(),rect.Height());

       size.cx -= size.cx % 4;
     
       // Количество битов на пиксел для текущего графического устройства
       int iBitCount = GetDeviceCaps(m_pDC->m_hDC, BITSPIXEL);
       // Количество цветовых плоскостей для текущего графического устройства
       int iPlanes = GetDeviceCaps(m_pDC->m_hDC, PLANES);
     
       // Рассчитываем кол-во необходимых байт для изображения
       int NbBytes = (iBitCount/8) * size.cx * size.cy;
       unsigned char *pPixelData = new unsigned char[NbBytes];
     
       // Определим формат копирования (в зависимости от количества битов на пиксел)
       // Здесь только для 24 и 32 битов. При необходимости можно добавить и для других значений
       GLenum glFormat;
       switch(iBitCount)
       {
       case 24:
          glFormat = GL_BGR_EXT;
          break;
       case 32:
          glFormat = GL_BGRA_EXT;
          break;
       }
       // Копируем из OpenGL
       ::glReadPixels(0,0, size.cx, size.cy, glFormat, GL_UNSIGNED_BYTE, pPixelData);
     
       // Заполняем заголовок растрового изображения
       BITMAPINFOHEADER header;
       header.biWidth = size.cx;
       header.biHeight = size.cy;
       header.biSizeImage = NbBytes;
       header.biSize = sizeof(BITMAPINFOHEADER);
       header.biPlanes = iPlanes;
       header.biBitCount =  iBitCount;
       header.biCompression = 0;
       header.biXPelsPerMeter = 0;
       header.biYPelsPerMeter = 0;
       header.biClrUsed = 0;
       header.biClrImportant = 0;
      
       // Генерируем handle
       HANDLE handle = (HANDLE)::GlobalAlloc (GHND, sizeof(BITMAPINFOHEADER) + NbBytes);
       char *pData;
       if(handle != NULL)
       {
          // Блокируем handle
          pData = (char *) ::GlobalLock((HGLOBAL)handle);
          // Копируем заголовок и данные
          memcpy(pData, &header, sizeof(BITMAPINFOHEADER));
          memcpy(pData + sizeof(BITMAPINFOHEADER), pPixelData, NbBytes);
          // Разблокируем
          ::GlobalUnlock((HGLOBAL)handle);

          // Кладём DIB в clipboard
          OpenClipboard();
          EmptyClipboard();
          SetClipboardData(CF_DIB, handle);
          CloseClipboard();
          }
     
          CBitmap oBmp;
          // Создаем растровое изображение, совместимое с графическим устройством
          // Оно пока не содержит рисунка
          oBmp.CreateCompatibleBitmap(m_pDC, size.cx, size.cy);
          // Устанавливаем битовый массив в растровое изображение
          oBmp.SetBitmapBits(NbBytes, pPixelData);
     
          delete [] pPixelData;
     
          EndWaitCursor();
    }


*/