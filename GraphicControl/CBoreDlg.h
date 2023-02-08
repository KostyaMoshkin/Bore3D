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

#define IDD_DIALOG_HIDDEN               131
#define IDC_CUSTOM1                     1009

class CBore3DtestDlg;

class GRAPHICCONTROL_API CBoreDlg : public CDialog, public IBore3D
{
public:
	enum { IDD = IDD_DIALOG_HIDDEN};

	CBoreDlg();
	~CBoreDlg();

public:
    BOOL Create(LPCSTR DialogName, CWnd* Owner = 0);
    BOOL OnInitDialog();

public:
    void fillPicture(HDC hDC_);
    // прототип метода дл€ отображени€ части 3D-ствола скважины в bitmap
    // 
    bool InitBore3D(
        IBoreData* pData, // интерфейс доступа к данным развЄртки
        float fLogPerPixel // коэффициент соотношени€ между логическими единицами (используютс€ маппером) и пиксел€ми экрана
    );

    void InitDiaMapper(
        IDiaMapper* pMapper_ // отображение глубина <--> логические единицы по вертикали (не путать с пиксел€ми)
    );

    // палитра цветов, "нат€гиваема€" на диапозон значений радиусов fMinRadius, fMaxRadius);
    bool InitPalette(const std::vector<COLORREF>& vecPalette);

    int GetBitmap(
        const RECT* pVisualRect, // пр€моугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
        float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
        // совокупно следующие 4 параметра определ€ют шкалу дл€ отображени€ ридиусов (как значение радиуса преобразуетс€ в видимую толщину ствола)
        float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
        int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах дл€ минимального и максимального радиуса 
        float fIsometryAngle, // угол изометрической проекции
        bool bDrawMesh
    ) override;

public:
    void setBkgColor(float r_, float g_, float b_);
    void setMesColor(float r_, float g_, float b_);
    void setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

    afx_msg void OnSize(UINT nType, int cx, int cy);

private:

    // ѕеременна€, показывающа€, используетс€ ли в
    // данный момент диалог

    BOOL inUse;
    CBore3DtestDlg* owner;
    DECLARE_MESSAGE_MAP()

    // ”наследовано через IBore3D
    virtual BOOL Create(LPCSTR DialogName, HWND hwndOwner) override;
    virtual void SetPosition(int cx, int cy) override;

private:
    struct Implementation;
    std::shared_ptr<Implementation> m_pImpl;
};

