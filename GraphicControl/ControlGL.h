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
		HDC m_saveHDC = nullptr;
		HGLRC m_rendContext = nullptr;

		bool m_bNeedUpdate = true;

		std::vector<byte> m_vPrintScreen;

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

	public:
		bool init();
		void needUpdate(bool bUpdate_ = TRUE) { m_bNeedUpdate = bUpdate_;  }

		void fillPicture(HDC hDC_, int nSizeX_, int nSizeY_);

	protected:
		bool initWindow();
		bool initGlew();
		bool createContext();
		bool makeCurrent() const;

		bool beginDraw(int width_, int height_) const;
		void endDraw() const;
		void OnPaint(bool bSave_);
		void SavePicture();

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