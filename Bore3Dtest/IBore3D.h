#pragma once

#ifdef GRAPHICCONTROL_EXPORTS
#define GRAPHICCONTROL_API __declspec(dllexport)
#else
#define GRAPHICCONTROL_API __declspec(dllimport)
#endif

#include <afxwin.h>

#include <IBoreData.h>
#include <IDiaMapper.h>

interface IBore3D
{
public:
    // ������������� �������� ����
    virtual BOOL Create(LPCSTR DialogName, HWND hwndOwner = NULL) = 0; // ������� ������� ����
    virtual void SetPosition(int cx, int cy) = 0; // ������ ������� �������� ����


    // �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
    virtual bool InitBore3D(
        IBoreData* pData, // ��������� ������� � ������ ��������
        float fLogPerPixel // ����������� ����������� ����� ����������� ��������� (������������ ��������) � ��������� ������
    ) = 0;

    virtual void InitDiaMapper(
        IDiaMapper* pMapper_ // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
    ) = 0;


    virtual int GetBitmap(
        const RECT* pVisualRect, // ������������� � ���������� �������� ������������ ����� 3D-������ (top,bottom ������������� fTop,fBottom ��� �������������� � pMapper)
        //!!    ������� ��������� 2 ���������, �.�. ��� ��������� ���������� � ���� ����, ��� fTop, fBottom ���������� �� pVisualRect ����� pMapper
        //float fTop, float fBottom, // �������� ������ (����) ����������� ������ ��������
        float fRotation, // �������������� ���� �������� ����� ������ ������ ����� ���
        // ��������� ��������� 4 ��������� ���������� ����� ��� ����������� �������� (��� �������� ������� ������������� � ������� ������� ������)
        float fMinRadius, float fMaxRadius, // ����������� � ������������ �������� �������/��������, ��������������� ������ 
        int nMinRadiusLP, int nMaxRadiusLP, // ������� ������ � ���������� �������� ��� ������������ � ������������� ������� 
        float fIsometryAngle, // ���� �������������� ��������
        bool bDrawMesh
    ) = 0;

    // !! ��������� �������� ��������������� ��������� �� �����, ������������ HBITMAP (����� ����������� � ������ ��������)
    //virtual HBITMAP fillPicture(HDC hDC_) = 0;
    virtual void fillPicture(HDC hDC_) = 0;

    // ������� ������, "������������" �� �������� �������� �������� fMinRadius, fMaxRadius);
    virtual bool InitPalette(const std::vector<COLORREF>& vecPalette) = 0;
    virtual void setBkgColor(float r_, float g_, float b_) = 0;
    virtual void setMesColor(float r_, float g_, float b_) = 0;
    virtual void setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1) = 0;

    GRAPHICCONTROL_API
        static IBore3D* Create3DBore(); // ����������� ��������� �����, ��������� ������, ����������� ��������� IBore3D
};