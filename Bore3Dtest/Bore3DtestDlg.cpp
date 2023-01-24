﻿
#include "pch.h"

#include "Bore3Dtest.h"
#include "Bore3DtestDlg.h"
#include "resource.h"
#include "afxdialogex.h"
#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CBore3DtestDlg



CBore3DtestDlg::CBore3DtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BORE3DTEST_DIALOG, pParent), m_hiddenDlg()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pParent = pParent;
}

CBore3DtestDlg::~CBore3DtestDlg()
{
}

void CBore3DtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBore3DtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TEST_GL, &CBore3DtestDlg::BN_OPENGL_CLICKED)
END_MESSAGE_MAP()


// Обработчики сообщений CBore3DtestDlg

BOOL CBore3DtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	m_hiddenDlg.Create("SAMPLEDIALOG", this);

	m_hiddenDlg.setBkgColor(1, 1, 1);
	m_hiddenDlg.setMesColor(0, 0, 0);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CBore3DtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CBore3DtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	BN_OPENGL_CLICKED();
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CBore3DtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBore3DtestDlg::BN_OPENGL_CLICKED()
{
	CWnd* pWnd = GetDlgItem(IDC_PICTURE_BOX);
	CDC* pCHC = pWnd->GetDC();
	HDC hDC = *pCHC;

	CRect clientRect;
	//pWnd->GetWindowRect(&rect);
	pWnd->GetClientRect(&clientRect);

	m_hiddenDlg.SetWindowPos(NULL, 0, 0, clientRect.Width(), clientRect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE); //

	RECT rcVisualRect;
	rcVisualRect.left = -clientRect.Width();
	rcVisualRect.right = clientRect.Width();
	rcVisualRect.top = 0;
	rcVisualRect.bottom = clientRect.Height();

	m_fRotationAngle += 3.1415926f / 100.0f;

	float fIsometryAngle = 2.0f * 3.14f / 360.0f * 15.0f + m_fRotationAngle / 5.0f;

	/*
	int GetBitmap(
		const RECT * pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
		float fTop, float fBottom, // интервал глубин (окно) отображения ствола скважины
		float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
		// совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
		float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
		int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
		float fIsometryAngle, // угол изометрической проекции
		bool bDrawMesh
	);
	*/

	m_hiddenDlg.GetBitmap(&rcVisualRect, 420.0f, 500.0f, m_fRotationAngle, 0.0f, 4.1f, 0.0f, float(clientRect.Width()), fIsometryAngle, true);
	m_hiddenDlg.fillPicture(hDC);

	return;

}
