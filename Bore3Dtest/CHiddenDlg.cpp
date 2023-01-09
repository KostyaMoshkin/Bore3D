#include "pch.h"
#include "CHiddenDlg.h"

CHiddenDlg::CHiddenDlg()
{
}

CHiddenDlg::~CHiddenDlg()
{
}

BOOL CHiddenDlg::Create(LPCSTR DialogName, CWnd* Owner)
{
    // Если диалог уже используется (был отображен),
        // то только отобразить его
    if (inUse) {


        m_controlGL.InitBore3D(m_pData, 0.0f);

        std::vector<COLORREF> vPalette;
        m_controlGL.InitPalette(vPalette);

        m_controlGL.init();




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
	return 0;
}

void CHiddenDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_CUSTOM1, m_controlGL);
}


