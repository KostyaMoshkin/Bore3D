#pragma once
#include "pch.h"

#include "ControlGL.h"
#include "RenderBoreSurface.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <array>
#include <vector>

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
		bool m_bDataInit = false;
		bool m_bPaletteInit = false;

		GL::RenderBoreSurfacePtr m_pRenderBoreSurface;

		DataProvider::IBoreData* m_pData;

	public:
		BoreControl();

	public:
		// Унаследовано через ControlGL
		void paint() override;

	public:
		// прототип метода для отображения части 3D-ствола скважины в bitmap
		// 
		bool InitBore3D(
			DataProvider::IBoreData* pData, // интерфейс доступа к данным развёртки
			float fLogPerPixel // коэффициент соотношения между логическими единицами (используются маппером) и пикселями экрана
		);

		//bool InitWindow(
		//	HWND hWnd, // хэндл скрытого окна для проведения теневых отрисовок в OpenGL (если надо)
		//	int nWidth,
		//	int nHeight
		//);

		// палитра цветов, "натягиваемая" на диапозон значений радиусов fMinRadius, fMaxRadius);
		bool InitPalette(const std::vector<COLORREF>& vecPalette);

		int GetBitmap(
			const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
			DataProvider::IDiaMapper* pMapper, // отображение глубина <--> логические единицы по вертикали (не путать с пикселями)
			float fTop, float fBottom, // интервал глубин (окно) отображения ствола скважины
			float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
			// совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
			float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
			int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
			float fIsometryAngle, // угол изометрической проекции
			bool bDrawMesh
		);
	};

}