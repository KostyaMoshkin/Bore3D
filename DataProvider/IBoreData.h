#pragma once

#include "dataprovider.h"

#include <vector>

namespace DataProvider
{

	// интерфейс абстрактного поставщика данных для отображения 3D-ствола скважины
	// данные представляются в виде массивов значений по глубинам для
	// радиусов или удвоенных радиусов (диаметров), располагающихся равномерно вокруг оси ствола скважины
	// на каждой глубине также задан поворот всей системы радиусов относительно условного направления на север
	class DATAPROVIDER_API IBoreData
	{
		CDataProvider m_dataProvider;

	public:
		IBoreData(const char* sFileName_);
		virtual ~IBoreData();

	public:
		int GetCurveCount(); // возвращает кол-во кривых радиусов
		const std::vector<float>& GetDepths(); // возвращает монотонный массив глубин  (возрастающий или убывающий)
		const std::vector<float>& GetRadiusCurve(int iRadius); // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин
		const std::vector<float>& GetRotation(); // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север
		bool IsDiameters(); // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы
	};
}