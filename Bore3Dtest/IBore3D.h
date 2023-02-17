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
    // Инициализация скрытого окна
    virtual BOOL Create(LPCSTR DialogName) = 0; // создать скрытое окно
    virtual void SetPosition(int cx, int cy) = 0; // задать размеры скрытого окна


    // прототип метода для отображения части 3D-ствола скважины в bitmap
    virtual bool InitBore3D(
        IBoreData* pData, // интерфейс доступа к данным развёртки
        float fLogPerPixel // коэффициент соотношения между логическими единицами (используются маппером) и пикселями экрана
    ) = 0;

    virtual bool InitDiaMapper(
        IDiaMapper* pMapper_ // отображение глубина <--> логические единицы по вертикали (не путать с пикселями)
    ) = 0;


    virtual int GetBitmap(
        const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
        //!!    Убираем следующие 2 параметра, т.к. они дублируют предыдущий в силу того, что fTop, fBottom получается из pVisualRect через pMapper
        //float fTop, float fBottom, // интервал глубин (окно) отображения ствола скважины
        float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
        // совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
        float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
        int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
        float fIsometryAngle, // угол изометрической проекции
        bool bDrawMesh
    ) = 0;

    // !! Предлагаю заменить непосредственно рисование на метод, возвращающий HBITMAP (можно рассмотреть и другие варианты)
    //virtual HBITMAP fillPicture(HDC hDC_) = 0;
    virtual bool fillPicture(void * pBuffer, size_t nSize, int nBufferType) = 0;

    // палитра цветов, "натягиваемая" на диапозон значений радиусов fMinRadius, fMaxRadius);
    virtual bool InitPalette(const std::vector<COLORREF>& vecPalette) = 0;
    virtual void setBkgColor(float r_, float g_, float b_) = 0;
    virtual void setMesColor(float r_, float g_, float b_) = 0;
    virtual void setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1) = 0;

    GRAPHICCONTROL_API
        static IBore3D* Create3DBore(); // статический фабричный метод, создающая объект, реализующий интерфейс IBore3D
};
