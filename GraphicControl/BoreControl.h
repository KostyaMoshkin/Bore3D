#pragma once
#include "pch.h"

#include "ControlGL.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <array>
#include <vector>
#include <memory>

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
	public:
		BoreControl();
		~BoreControl();

	public:
		// Унаследовано через ControlGL
		void paint() override;
		bool init() override;

	public:
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
		);

	public:
		bool setBkgColor(float r_, float g_, float b_);
		bool setMesColor(float r_, float g_, float b_);
		bool setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1);

	private:
		struct Implementation;
		std::shared_ptr<Implementation> m_pImpl;
	};

}