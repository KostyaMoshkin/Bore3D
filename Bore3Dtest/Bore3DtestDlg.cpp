
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

	CRect rect;
	pWnd->GetWindowRect(&rect);

	m_hiddenDlg.SetWindowPos(NULL, 0, 0, rect.Width() + 15, rect.Height() + 37, SWP_NOMOVE | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE); //

	RECT rcVisualRect;
	rcVisualRect.left = 0;
	rcVisualRect.right = 0;
	rcVisualRect.top = 0;
	rcVisualRect.bottom = 100;

	m_hiddenDlg.GetBitmap(&rcVisualRect, 0, 500, 0.0f, 0.0f, 5.0f, 0, 100, 15.0f, false);
	m_hiddenDlg.fillPicture(hDC);
	/*
	//m_hiddenDlg.ShowWindow();

	//CRect rect;
	//CWnd* pWnd = GetDlgItem(IDC_CUSTOM1);
	//pWnd->GetWindowRect(&rect);

	//int     pixelFormat;

	//PIXELFORMATDESCRIPTOR pfd = {
	//sizeof(PIXELFORMATDESCRIPTOR), // Size of this structure
	//1, // Version of this structure
	//PFD_DRAW_TO_WINDOW | // Draw to window (not bitmap) PFD_DRAW_TO_BITMAP PFD_DRAW_TO_WINDOW
	//PFD_SUPPORT_OPENGL | // Support OpenGL calls
	//PFD_DOUBLEBUFFER, // Double -buffered mode
	//PFD_TYPE_RGBA, // RGBA Color mode
	//24, // Want 24bit color
	//0,0,0,0,0,0, // Not used to select mode
	//0,0, // Not used to select mode
	//0,0,0,0,0, // Not used to select mode
	//32, // Size of depth buffer
	//0, // Not used to select mode
	//0, // Not used to select mode
	//PFD_MAIN_PLANE, // Draw in main plane
	//0, // Not used to select mode
	//0,0,0 }; // Not used to select mode

	////HDC memDC = CreateCompatibleDC(NULL);
	//CDC* dc;// = GetDlgItem(IDC_PICTURE)->GetDC();
	//dc = new CClientDC(this);
	//HDC memDC = *dc;


	//HBITMAP memBM = CreateCompatibleBitmap(memDC, rect.Width(), rect.Height());
	//SelectObject(memDC, memBM);
	//pixelFormat = ChoosePixelFormat(memDC, &pfd);
	//SetPixelFormat(memDC, pixelFormat, &pfd);
	//HGLRC m_hrc = wglCreateContext(memDC);
	//wglMakeCurrent(memDC, m_hrc);

	//GLenum err = glewInit();
	//if (err != GLEW_OK)
	//{
	//	return;
	//}

	//wglewInit();

	//glViewport(0, 0, rect.Width(), rect.Height());

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();

	//glClearColor(0.2f, 0.0f, 0.3f, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColor3f(0.0f, 0.5f, 1.0f); //blue color

	//glBegin(GL_TRIANGLES);//start drawing triangles
	//glVertex3f(-1.0f, -0.25f, 0.0f);//triangle one first vertex
	//glVertex3f(-0.5f, -0.25f, 0.0f);//triangle one second vertex
	//glVertex3f(-0.75f, 0.25f, 0.0f);//triangle one third vertex
	////drawing a new triangle

	//glColor3f(0.0f, 0, 0.5f); //blue color
	//glVertex3f(0.5f, -0.25f, 0.0f);//triangle two first vertex
	//glVertex3f(1.0f, -0.25f, 0.0f);//triangle two second vertex
	//glVertex3f(0.75f, 0.25f, 0.0f);//triangle two third vertex
	//glEnd();//end drawing of triangles

	//glBegin(GL_POLYGON);
	//glColor3f(1, 0, 0); glVertex3f(-0.6f, -0.75f, 0.5f);
	//glColor3f(1, 0.5f, 1); glVertex3f(0.6f, -0.75f, 0.0f);
	//glColor3f(0, 0, 1); glVertex3f(0.0f, 0.75f, 0.0f);
	//glEnd();

	//glFlush();

	//std::vector<uint32_t> colours(rect.Width() * rect.Height());

	//glReadPixels(0, 0, rect.Width(), rect.Height(),
	//	GL_RGBA,
	//	GL_UNSIGNED_INT_8_8_8_8,
	//	colours.data());


	//wglMakeCurrent(memDC, m_hrc);

	//wglDeleteContext(m_hrc);
	*/

	return;

}
