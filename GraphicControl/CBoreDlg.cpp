#include "pch.h"
#include "CBoreDlg.h"

#include "BoreControl.h"

#include <memory>

struct CBoreDlg::Implementation
{
    GraphicControl::BoreControl m_controlGL;
};

CBoreDlg::CBoreDlg()
{
    m_pImpl = std::make_shared<Implementation>();
}

CBoreDlg::~CBoreDlg()
{
}

BOOL CBoreDlg::Create(LPCSTR DialogName, CWnd* Owner)
{
    // Если диалог уже используется (был отображен),
        // то только отобразить его
    if (inUse) {
        this->ShowWindow(SW_SHOW);
        return TRUE;
    }

    inUse = TRUE; // Диалог используется
    // Создать диалог и получить результат создания
    BOOL success = (CDialog::Create(CBoreDlg::IDD, Owner) != FALSE);
    owner = (CBore3DtestDlg*)Owner; // Собственник
    return success;
}

BOOL CBoreDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (!m_pImpl->m_controlGL.init())
        return 0;

	return 0;
}

void CBoreDlg::fillPicture(HDC hDC_)
{
    m_pImpl->m_controlGL.fillPicture(hDC_);
}

bool CBoreDlg::InitBore3D(IBoreData* pData, float fLogPerPixel)
{
    return m_pImpl->m_controlGL.InitBore3D(pData, fLogPerPixel);
}

void CBoreDlg::InitDiaMapper(IDiaMapper* pMapper_)
{
    m_pImpl->m_controlGL.InitDiaMapper(pMapper_);
}

bool CBoreDlg::InitPalette(const std::vector<COLORREF>& vecPalette)
{
    return m_pImpl->m_controlGL.InitPalette(vecPalette);
}

int CBoreDlg::GetBitmap(const RECT* pVisualRect, float fTop, float fBottom, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
{
    m_pImpl->m_controlGL.GetBitmap(pVisualRect, fTop, fBottom, fRotation, fMinRadius, fMaxRadius, nMinRadiusLP, nMaxRadiusLP, fIsometryAngle, bDrawMesh);
    return 0;
}

void CBoreDlg::setBkgColor(float r_, float g_, float b_)
{
    m_pImpl->m_controlGL.setBkgColor(r_, g_, b_);
}

void CBoreDlg::setMesColor(float r_, float g_, float b_)
{
    m_pImpl->m_controlGL.setMesColor(r_, g_, b_);
}

void CBoreDlg::setZeroLineColor(float r_, float g_, float b_, int nWidth_)
{
    m_pImpl->m_controlGL.setZeroLineColor(r_, g_, b_, nWidth_);
}

void CBoreDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_CUSTOM1, m_pImpl->m_controlGL);
}

void CBoreDlg::OnSize(UINT nType, int cx, int cy)
{
    if (!GetDlgItem(IDC_CUSTOM1)) 
        return;

    GetDlgItem(IDC_CUSTOM1)->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}

BEGIN_MESSAGE_MAP(CBoreDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()
