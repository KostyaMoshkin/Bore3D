#pragma once

#include "IBore3D.h"

#include "BoreData.h"
#include "DiaMapper.h"

#include <memory>

// Диалоговое окно CBore3DtestDlg
class CBore3DtestDlg : public CDialogEx
{
	CWnd* m_pParent;

	IBore3D* m_boreGL;

	bool m_bBoreGLInit = false;

	float m_fRotationAngle = 0;

	int m_nGepthShift = 0;

	std::vector<byte> m_vPrintScreen;

	std::shared_ptr<BoreData> m_pData = nullptr;
	std::shared_ptr<DiaMapper> m_pDia = nullptr;

	CWnd* m_pWnd = nullptr;
	HDC m_hDC = nullptr;

	CRect m_clientRect;

	// Создание
public:
	CBore3DtestDlg(CWnd* pParent = nullptr);	// стандартный конструктор
	~CBore3DtestDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BORE3DTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

protected:
	void Bore3DPaint();

// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void BN_OPENGL_CLICKED();
};
