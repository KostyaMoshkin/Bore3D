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
        const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
        float fTop, float fBottom, // интервал глубин (окно) отображения ствола скважины
        float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
        // совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
        float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
        int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
        float fIsometryAngle, // угол изометрической проекции
        bool bDrawMesh
    );

public:
    void setBkgColor(float r_, float g_, float b_);
    void setMesColor(float r_, float g_, float b_);
    void setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

    afx_msg void OnSize(UINT nType, int cx, int cy);

private:

    // Переменная, показывающая, используется ли в
    // данный момент диалог

    BOOL inUse;
    CBore3DtestDlg* owner;
    DECLARE_MESSAGE_MAP()

private:
    struct Implementation;
    std::shared_ptr<Implementation> m_pImpl;

};

