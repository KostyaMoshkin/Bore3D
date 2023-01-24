#pragma once

#ifdef GRAPHICCONTROL_EXPORTS
#define GRAPHICCONTROL_API __declspec(dllexport)
#else
#define GRAPHICCONTROL_API __declspec(dllimport)
#endif
#include <afxwin.h>

#include "resource.h"

#include "BoreControl.h"

#define IDD_DIALOG_HIDDEN               131
#define IDC_CUSTOM1                     1009

class CBore3DtestDlg;

class GRAPHICCONTROL_API CHiddenDlg : public CDialog
{
    GraphicControl::BoreControl m_controlGL;

public:
	enum { IDD = IDD_DIALOG_HIDDEN};

	CHiddenDlg();
	~CHiddenDlg();

public:
    BOOL Create(LPCSTR DialogName, CWnd* Owner = 0);
    BOOL OnInitDialog();

public:
    void fillPicture(HDC hDC_);

    int GetBitmap(
        const RECT* pVisualRect, // ������������� � ���������� �������� ������������ ����� 3D-������ (top,bottom ������������� fTop,fBottom ��� �������������� � pMapper)
        float fTop, float fBottom, // �������� ������ (����) ����������� ������ ��������
        float fRotation, // �������������� ���� �������� ����� ������ ������ ����� ���
        // ��������� ��������� 4 ��������� ���������� ����� ��� ����������� �������� (��� �������� ������� ������������� � ������� ������� ������)
        float fMinRadius, float fMaxRadius, // ����������� � ������������ �������� �������/��������, ��������������� ������ 
        int nMinRadiusLP, int nMaxRadiusLP, // ������� ������ � ���������� �������� ��� ������������ � ������������� ������� 
        float fIsometryAngle, // ���� �������������� ��������
        bool bDrawMesh
    );

public:
    void setBkgColor(float r_, float g_, float b_);
    void setMesColor(float r_, float g_, float b_);
    void setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV

    afx_msg void OnSize(UINT nType, int cx, int cy);

private:

    // ����������, ������������, ������������ �� �
    // ������ ������ ������

    BOOL inUse;
    CBore3DtestDlg* owner;
    DECLARE_MESSAGE_MAP()

private:
    struct Implementation;
    std::shared_ptr<Implementation> m_pImpl;

};

