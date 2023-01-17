#include "pch.h"
#include "CHiddenDlg.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <memory>

struct CHiddenDlg::Implementation
{
    std::shared_ptr<DataProvider::BoreData> m_pData = nullptr;
    std::shared_ptr<DataProvider::IDiaMapper> m_pDia = nullptr;
};

CHiddenDlg::CHiddenDlg()
{
    m_pImpl = std::make_shared<Implementation>();
    m_pImpl->m_pData = std::make_shared<DataProvider::BoreData>("E:\\VisualStudioProjects\\Bore3D\\3D-развёртка.txt");
    m_pImpl->m_pDia = std::make_shared<DataProvider::IDiaMapper>();
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

    if (!m_controlGL.init())
        return 0;

    m_controlGL.InitBore3D(m_pImpl->m_pData.get(), 1.0f);

    m_pImpl->m_pDia->SetGeoRangeLPRange(10, 1000, 100, 300);
    m_controlGL.InitDiaMapper(m_pImpl->m_pDia.get());

    std::vector<COLORREF> vPalette;
    vPalette.push_back(0x00000000);
    vPalette.push_back(0x00FF0000);
    vPalette.push_back(0x00FFFF00);
    vPalette.push_back(0x0000FF00);
    vPalette.push_back(0x0000FFFF);
    vPalette.push_back(0x000000FF);
    vPalette.push_back(0x00FF00FF);
    vPalette.push_back(0x00FFFFFF);
    m_controlGL.InitPalette(vPalette);

	return 0;
}

void CHiddenDlg::fillPicture(HDC hDC_)
{
    m_controlGL.fillPicture(hDC_);
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

    GetDlgItem(IDC_CUSTOM1)->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}

BEGIN_MESSAGE_MAP(CHiddenDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()
