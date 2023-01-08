
// Bore3DtestDlg.h: файл заголовка
//

#pragma once

#include <DataProvider.h>

#include "BoreControl.h"


// Диалоговое окно CBore3DtestDlg
class CBore3DtestDlg : public CDialogEx
{

	CWnd* m_pParent;

	GraphicControl::BoreControl m_controlGL;

	DataProvider::CDataProvider m_dataProvider;

	// Создание
public:
	CBore3DtestDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BORE3DTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void BN_OPENGL_CLICKED();
};
