#pragma once
#include <afxwin.h>

#include "resource.h"

#include "BoreControl.h"

class CBore3DtestDlg;

class CHiddenDlg : public CDialog
{
    GraphicControl::BoreControl m_controlGL;
    DataProvider::BoreData* m_pData = nullptr;

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
    void fillPicture(HDC hDC_, int nSizeX_, int nSizeY_);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

    afx_msg void OnSize(UINT nType, int cx, int cy);

private:

    // Переменная, показывающая, используется ли в
    // данный момент диалог

    BOOL inUse;
    CBore3DtestDlg* owner;
    DECLARE_MESSAGE_MAP()
};

