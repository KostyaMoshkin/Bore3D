#pragma once

#ifdef GRAPHICCONTROL_EXPORTS
#define GRAPHICCONTROL_API __declspec(dllexport)
#else
#define GRAPHICCONTROL_API __declspec(dllimport)
#endif
#include <afxwin.h>

#include "resource.h"

#include "BoreControl.h"

#define IDD_DIALOG_HIDDEN               131
#define IDC_CUSTOM1                     1009

class CBore3DtestDlg;

class GRAPHICCONTROL_API CHiddenDlg : public CDialog
{
    GraphicControl::BoreControl m_controlGL;

    int n_mWindowSizeX = 0;
    int n_mWindowSizeY = 0;

public:
	enum { IDD = IDD_DIALOG_HIDDEN};

	CHiddenDlg();
	~CHiddenDlg();

public:
    BOOL Create(LPCSTR DialogName, CWnd* Owner = 0);
    BOOL OnInitDialog();

public:
    void fillPicture(HDC hDC_);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

    afx_msg void OnSize(UINT nType, int cx, int cy);

private:

    // Переменная, показывающая, используется ли в
    // данный момент диалог

    BOOL inUse;
    CBore3DtestDlg* owner;
    DECLARE_MESSAGE_MAP()

private:
    struct Implementation;
    std::shared_ptr<Implementation> m_pImpl;

};

