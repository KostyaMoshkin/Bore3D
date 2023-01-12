#include "pch.h"
#include "CHiddenDlg.h"

#include <IBoreData.h>

#include <memory>

struct CHiddenDlg::Implementation
{
    std::shared_ptr<DataProvider::BoreData> m_pData = nullptr;
};

CHiddenDlg::CHiddenDlg()
{
    m_pImpl = std::make_shared<Implementation>();
    m_pImpl->m_pData = std::make_shared<DataProvider::BoreData>("E:\\VisualStudioProjects\\Bore3D\\3D-развёртка.txt");
}

CHiddenDlg::~CHiddenDlg()
{
}

BOOL CHiddenDlg::Create(LPCSTR DialogName, CWnd* Owner)
{
    // Если диалог уже используется (был отображен),
        // то только отобразить его
    if (inUse) {
        this->ShowWindow(SW_SHOW);
        return TRUE;
    }

    inUse = TRUE; // Диалог используется
    // Создать диалог и получить результат создания
    BOOL success = (CDialog::Create(CHiddenDlg::IDD, Owner) != FALSE);
    owner = (CBore3DtestDlg*)Owner; // Собственник
    return success;
}

BOOL CHiddenDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_controlGL.init();

    m_controlGL.InitBore3D(m_pImpl->m_pData.get(), 0.0f);

    std::vector<COLORREF> vPalette;
    m_controlGL.InitPalette(vPalette);

	return 0;
}

void CHiddenDlg::fillPicture(HDC hDC_)
{
    m_controlGL.fillPicture(hDC_, n_mWindowSizeX, n_mWindowSizeY);
}

void CHiddenDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_CUSTOM1, m_controlGL);
}

void CHiddenDlg::OnSize(UINT nType, int cx, int cy)
{
    if (!GetDlgItem(IDC_CUSTOM1)) 
        return;

    n_mWindowSizeX = cx;
    n_mWindowSizeY = cy;

    GetDlgItem(IDC_CUSTOM1)->SetWindowPos(NULL, 0, 0, n_mWindowSizeX, n_mWindowSizeY, SWP_NOZORDER);
}

BEGIN_MESSAGE_MAP(CHiddenDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()
