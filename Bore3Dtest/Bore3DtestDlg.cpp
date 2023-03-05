
#include "pch.h"

#include "Bore3Dtest.h"
#include "Bore3DtestDlg.h"
#include "resource.h"
#include "afxdialogex.h"
#include <WinUser.h>
#include <chrono>
#include <string>

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


CBore3DtestDlg::CBore3DtestDlg(CWnd* pParent)
	: CDialogEx(IDD_BORE3DTEST_DIALOG, pParent), m_clientRect(0, 0, 0, 0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_boreGL = IBore3D::Create3DBore();
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

	if (!m_boreGL)
		return false;

	m_pData = std::make_shared<BoreData>("Bore3D.txt");

	if (!m_pData->isInit())
		return false;

	m_pDia = std::make_shared<DiaMapper>();

	m_bBoreGLInit = m_boreGL->Create("OPENGL DIALOG");

	if (!m_bBoreGLInit)
	{
		m_sErrorMessage = m_boreGL->getErrorMessage();
		return false;
	}

	//  Второй параметр - индекси рисуемой горизонтальной линии. 1 - каждая линия, 10 - каждая десятая.
	m_bBoreGLInit = m_boreGL->InitBore3D(m_pData.get(), 10);

	if (!m_bBoreGLInit)
	{
		m_sErrorMessage = m_boreGL->getErrorMessage();
		return false;
	}

	m_pDia->SetGeoRangeLPRange(10, 1000, 100, 300);
	m_bBoreGLInit = m_boreGL->InitDiaMapper(m_pDia.get());

	if (!m_bBoreGLInit)
	{
		m_sErrorMessage = m_boreGL->getErrorMessage();
		return false;
	}

	std::vector<COLORREF> vPalette;
	vPalette.push_back(0x00000000);
	vPalette.push_back(0x00FF0000);
	vPalette.push_back(0x00FFFF00);
	vPalette.push_back(0x0000FF00);
	vPalette.push_back(0x0000FFFF);
	vPalette.push_back(0x000000FF);
	vPalette.push_back(0x00FF00FF);
	vPalette.push_back(0x00FFFFFF);

	m_bBoreGLInit = m_boreGL->InitPalette(vPalette);

	if (!m_bBoreGLInit)
	{
		m_sErrorMessage = m_boreGL->getErrorMessage();
		return false;
	}

	m_boreGL->setBkgColor(1, 1, 1);
	m_boreGL->setMesColor(0, 0, 0);
	m_boreGL->setZeroLineColor(0.8f, 0.8f, 0.8f, 3);

	m_pWnd = GetDlgItem(IDC_PICTURE_BOX);
	CDC* pCHC = m_pWnd->GetDC();
	m_hDC = *pCHC;

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

	Bore3DPaint();
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CBore3DtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBore3DtestDlg::Bore3DPaint()
{
	if (!m_bBoreGLInit)
		return;

	CRect clientRect;
	m_pWnd->GetClientRect(&m_clientRect);

	if (clientRect.Width() != m_clientRect.Width() || clientRect.Height() != m_clientRect.Height())
	{
		m_boreGL->SetPosition(m_clientRect.Width(), m_clientRect.Height());
		m_vPrintScreen.resize(m_clientRect.Width() * m_clientRect.Height() * 4);
	}

	RECT rcVisualRect;
	rcVisualRect.left = -m_clientRect.Width();
	rcVisualRect.right = m_clientRect.Width();
	rcVisualRect.top = 460 + m_nGepthShift;
	rcVisualRect.bottom = 800 + m_nGepthShift;

	float fIsometryAngle = 15.0f;// +m_fRotationAngle / 50.0f;

	m_boreGL->GetBitmap(&rcVisualRect, m_fRotationAngle, 2.0f, 4.1f, 0, m_clientRect.Width(), fIsometryAngle, true);


#define GL_BGRA 0x80E1

	m_boreGL->fillPicture(m_vPrintScreen.data(), m_clientRect.Width() * m_clientRect.Height(), GL_BGRA);

	HBITMAP resultBitmap = CreateBitmap(m_clientRect.Width(), m_clientRect.Height(), 1, 32, (void*)m_vPrintScreen.data());
	HDC MemDC = CreateCompatibleDC(m_hDC);
	auto hOldBmp = SelectObject(MemDC, resultBitmap);
	BitBlt(m_hDC, 0, 0, m_clientRect.Width(), m_clientRect.Height(), MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, hOldBmp);
	DeleteDC(MemDC);
	DeleteObject(resultBitmap);

	return;
}

void CBore3DtestDlg::BN_OPENGL_CLICKED()
{
	int nCount = 0;

	auto start = std::chrono::steady_clock::now();
	bool bRun = true;

	float fTimeLength = 5.0f;

	while (bRun)
	{
		Bore3DPaint();

		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		bRun = elapsed_seconds.count() < fTimeLength;

		m_fRotationAngle = 360.0f * float(elapsed_seconds.count() / fTimeLength);

		++nCount;

		++m_nGepthShift;
	}

	CString sFPS;
	sFPS.Format(_T("%d"), int(nCount / fTimeLength));

	SetDlgItemText(IDC_STATIC, LPCTSTR(sFPS));
}
