#pragma once
#include <afxwin.h>

#include "resource.h"

#include "BoreControl.h"

class CBore3DtestDlg;

class CHiddenDlg : public CDialog
{
    GraphicControl::BoreControl m_controlGL;
    DataProvider::BoreData* m_pData = nullptr;

public:
	enum { IDD = IDD_DIALOG_HIDDEN};

	CHiddenDlg();
	~CHiddenDlg();

    BOOL Create(LPCSTR DialogName, CWnd* Owner = 0);
    BOOL OnInitDialog();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

private:

    // Переменная, показывающая, используется ли в
    // данный момент диалог

    BOOL inUse;
    CBore3DtestDlg* owner;
    //DECLARE_MESSAGE_MAP()
};

