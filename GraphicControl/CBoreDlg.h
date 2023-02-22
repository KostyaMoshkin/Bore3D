#pragma once

#ifdef GRAPHICCONTROL_EXPORTS
#define GRAPHICCONTROL_API __declspec(dllexport)
#else
#define GRAPHICCONTROL_API __declspec(dllimport)
#endif
#include <afxwin.h>

#include "resource.h"

#include <memory>
#include <vector>

#include "IBore3D.h"

class CBore3DtestDlg;

class GRAPHICCONTROL_API CBoreDlg : public CDialog, public IBore3D
{
public:
	enum { IDD = IDD_GC_DIALOG_HIDDEN};

	CBoreDlg();
	~CBoreDlg();

public:
    bool fillPicture(void* pBuffer, size_t nSize, int nBufferType);
    // �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
    // 
    bool InitBore3D(
        IBoreData* pData, // ��������� ������� � ������ ��������
        int nMeshStep_ // ��� ����� ������� �������������� �����
    );

    bool InitDiaMapper(
        IDiaMapper* pMapper_ // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
    );

    // ������� ������, "������������" �� �������� �������� �������� fMinRadius, fMaxRadius);
    bool InitPalette(const std::vector<COLORREF>& vecPalette);

    int GetBitmap(
        const RECT* pVisualRect, // ������������� � ���������� �������� ������������ ����� 3D-������ (top,bottom ������������� fTop,fBottom ��� �������������� � pMapper)
        float fRotation, // �������������� ���� �������� ����� ������ ������ ����� ���
        // ��������� ��������� 4 ��������� ���������� ����� ��� ����������� �������� (��� �������� ������� ������������� � ������� ������� ������)
        float fMinRadius, float fMaxRadius, // ����������� � ������������ �������� �������/��������, ��������������� ������ 
        int nMinRadiusLP, int nMaxRadiusLP, // ������� ������ � ���������� �������� ��� ������������ � ������������� ������� 
        float fIsometryAngle, // ���� �������������� ��������
        bool bDrawMesh
    ) override;

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
    DECLARE_MESSAGE_MAP()

    // ������������ ����� IBore3D
    virtual BOOL Create(LPCSTR DialogName) override;
    virtual void SetPosition(int cx, int cy) override;

private:
    struct Implementation;
    std::shared_ptr<Implementation> m_pImpl;
};

