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

namespace GraphicControl
{

	class GRAPHICCONTROL_API ControlGL : public CWnd
	{
		CDC* m_cDC = nullptr;
		HDC m_hDC;
		HGLRC m_rendContext = nullptr;

		bool m_bNeedUpdate = true;

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

	protected:
		bool initWindow();
		bool initGlew();
		bool createContext();
		bool makeCurrent() const;

		bool beginDraw(int width_, int height_) const;
		void endDraw() const;


	protected:
		BOOL RegisterWindowClass();

		// Generated message map functions
	protected:
		//{{AFX_MSG(ControlGL)
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

}