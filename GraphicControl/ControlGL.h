#pragma once

#ifdef GRAPHICCONTROL_EXPORTS
#define GRAPHICCONTROL_API __declspec(dllexport)
#else
#define GRAPHICCONTROL_API __declspec(dllimport)
#endif

#ifndef CONTROLGL_CLASSNAME
#define CONTROLGL_CLASSNAME    _T("ControlGL")  // Window class name
#endif

/////////////////////////////////////////////////////////////////////////////
// ControlGL window

#include <vector>

namespace GraphicControl
{

	class GRAPHICCONTROL_API ControlGL : public CWnd
	{
		HDC m_GLHDC = nullptr;

		HGLRC m_rendContext = nullptr;

		void* m_pBuffer = nullptr;
		int m_nBufferType = 0;
		size_t m_nBufferSize = 0;

		bool m_bSavePictureResult = false;

		bool m_bNeedUpdate = true;

		int m_nWindowSizeX = 10;
		int m_nWindowSizeY = 10;

	protected:
		CPoint m_ptWindow;

		// Construction
	public:
		ControlGL();

		// Operations
	public:
		BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle = WS_VISIBLE);

		// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(ControlGL)
	protected:
		virtual void PreSubclassWindow();
		//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~ControlGL();

	public:
		virtual void paint() = 0;
		virtual bool init();

	public:
		void needUpdate(bool bUpdate_ = TRUE) { m_bNeedUpdate = bUpdate_;  }

		bool fillPicture(void * pBuffer, size_t nSize, int nBufferType);

	protected:
		bool initWindow();
		bool initGlew();
		bool createContext();
		bool makeCurrent() const;

		void OnPaint(bool bSave_);
		void SavePicture();

		bool beginDraw();
		bool beginDraw(int width_, int height_);
		void endDraw() const;

		void changeWindowSize(CRect rcWindow_);

	protected:
		BOOL RegisterWindowClass();

		// Generated message map functions
	protected:
		//{{AFX_MSG(ControlGL)
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

}