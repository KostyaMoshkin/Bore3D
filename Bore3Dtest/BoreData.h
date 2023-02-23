#pragma once
#include <IBoreData.h>

#include <dataprovider.h>

class BoreData : public IBoreData
{
	DataProvider::CDataProvider m_dataProvider;

	bool m_bInit = false;

public:
	BoreData() {}
	BoreData(const char* sFileName_);
	virtual ~BoreData();

public:
	bool isInit() { return m_bInit; }

	int GetCurveCount() override; // возвращает кол-во кривых радиусов
	const std::vector<float>& GetDepths() override; // возвращает монотонный массив глубин  (возрастающий или убывающий)
	const std::vector<float>& GetRadiusCurve(int iRadius) override; // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин
	const std::vector<float>& GetRotation() override; // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север
	bool IsDiameters() override; // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы
};

