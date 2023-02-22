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

BOOL CBoreDlg::Create(LPCSTR DialogName)
{
    // Если диалог уже используется (был отображен),
        // то только отобразить его
    if (inUse) {
        this->ShowWindow(SW_SHOW);
        return TRUE;
    }

    inUse = TRUE; // Диалог используется

    HINSTANCE hResourceOld = AfxGetResourceHandle();
    AfxSetResourceHandle(g_hResource);

    // Создать диалог и получить результат создания
    BOOL success = (CDialog::Create(CBoreDlg::IDD, nullptr) != FALSE);

    AfxSetResourceHandle(hResourceOld);

    if (!success)
        return false;

    if (!m_pImpl->m_controlGL.init())
        return false;

    return true;
}

bool CBoreDlg::fillPicture(void* pBuffer, size_t nSize, int nBufferType)
{
    return m_pImpl->m_controlGL.fillPicture(pBuffer, nSize, nBufferType);
}

bool CBoreDlg::InitBore3D(IBoreData* pData, int nMeshStep_)
{
    return m_pImpl->m_controlGL.InitBore3D(pData, nMeshStep_);
}

bool CBoreDlg::InitDiaMapper(IDiaMapper* pMapper_)
{
    return m_pImpl->m_controlGL.InitDiaMapper(pMapper_);
}

bool CBoreDlg::InitPalette(const std::vector<COLORREF>& vecPalette)
{
    return m_pImpl->m_controlGL.InitPalette(vecPalette);
}

int CBoreDlg::GetBitmap(const RECT* pVisualRect, float fRotation, float fMinRadius, float fMaxRadius, int nMinRadiusLP, int nMaxRadiusLP, float fIsometryAngle, bool bDrawMesh)
{
    m_pImpl->m_controlGL.GetBitmap(pVisualRect, fRotation, fMinRadius, fMaxRadius, nMinRadiusLP, nMaxRadiusLP, fIsometryAngle, bDrawMesh);
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
    HINSTANCE hResourceOld = AfxGetResourceHandle();
    AfxSetResourceHandle(g_hResource);

    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_GC_CUSTOM1, m_pImpl->m_controlGL);
    AfxSetResourceHandle(hResourceOld);
}

void CBoreDlg::OnSize(UINT /*nType*/, int cx, int cy)
{
    SetPosition(cx, cy);
}

void CBoreDlg::SetPosition(int cx, int cy)
{
    HINSTANCE hResourceOld = AfxGetResourceHandle();
    AfxSetResourceHandle(g_hResource);

    if (!GetDlgItem(IDC_GC_CUSTOM1))
        return;

    GetDlgItem(IDC_GC_CUSTOM1)->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE);
    AfxSetResourceHandle(hResourceOld);
}

IBore3D* IBore3D::Create3DBore()
{
    return static_cast<IBore3D*>(new CBoreDlg());
}

BEGIN_MESSAGE_MAP(CBoreDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()
