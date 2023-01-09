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

	public:
		virtual ~IBoreData() = default;

		virtual int GetCurveCount() = 0;
		virtual const std::vector<float>& GetDepths() = 0;
		virtual const std::vector<float>& GetRadiusCurve(int iRadius) = 0;
		virtual const std::vector<float>& GetRotation() = 0;
		virtual bool IsDiameters() = 0;
	};

	class DATAPROVIDER_API BoreData : public IBoreData
	{
		CDataProvider m_dataProvider;

	public :
		BoreData(const char* sFileName_);
		virtual ~BoreData();

	public:
		int GetCurveCount() override; // возвращает кол-во кривых радиусов
		const std::vector<float>& GetDepths() override; // возвращает монотонный массив глубин  (возрастающий или убывающий)
		const std::vector<float>& GetRadiusCurve(int iRadius) override; // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин
		const std::vector<float>& GetRotation() override; // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север
		bool IsDiameters() override; // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы
	};
}