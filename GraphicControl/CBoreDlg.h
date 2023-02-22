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
    // прототип метода для отображения части 3D-ствола скважины в bitmap
    // 
    bool InitBore3D(
        IBoreData* pData, // интерфейс доступа к данным развёртки
        int nMeshStep_ // шаг между линиями горизонтальной сетки
    );

    bool InitDiaMapper(
        IDiaMapper* pMapper_ // отображение глубина <--> логические единицы по вертикали (не путать с пикселями)
    );

    // палитра цветов, "натягиваемая" на диапозон значений радиусов fMinRadius, fMaxRadius);
    bool InitPalette(const std::vector<COLORREF>& vecPalette);

    int GetBitmap(
        const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
        float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
        // совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
        float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
        int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
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

    // Переменная, показывающая, используется ли в
    // данный момент диалог

    BOOL inUse;
    DECLARE_MESSAGE_MAP()

    // Унаследовано через IBore3D
    virtual BOOL Create(LPCSTR DialogName) override;
    virtual void SetPosition(int cx, int cy) override;

private:
    struct Implementation;
    std::shared_ptr<Implementation> m_pImpl;
};

