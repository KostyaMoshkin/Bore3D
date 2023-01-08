#pragma once

#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif

#include <vector>

namespace DataProvider
{

	// интерфейс абстрактного поставщика данных для отображения 3D-ствола скважины
	// данные представляются в виде массивов значений по глубинам для
	// радиусов или удвоенных радиусов (диаметров), располагающихся равномерно вокруг оси ствола скважины
	// на каждой глубине также задан поворот всей системы радиусов относительно условного направления на север
	class DATAPROVIDER_API IBoreData
	{
	public:
		int GetCurveCount() const { return 0; } // возвращает кол-во кривых радиусов
		const std::vector<float>& GetDepths() { std::vector<float> a; return a; } // возвращает монотонный массив глубин  (возрастающий или убывающий)
		const std::vector<float>& GetRadiusCurve(int iRadius) { std::vector<float> a; return a; } // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин
		const std::vector<float>& GetRotation() { std::vector<float> a; return a; } // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север
		bool IsDiameters() const { return true; } // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы
	};
}