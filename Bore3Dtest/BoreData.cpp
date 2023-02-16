#include "pch.h"
#include "BoreData.h"

BoreData::BoreData(const char* sFileName_)
{
	m_dataProvider.init(sFileName_);
}

BoreData::~BoreData()
{

}

int BoreData::GetCurveCount()
{
	return (int)m_dataProvider.getData()->vvfDistance[0].size();
} // возвращает кол-во кривых радиусов

const std::vector<float>& BoreData::GetDepths()
{
	std::vector<float> a;
	return m_dataProvider.getData()->vfDepth;
} // возвращает монотонный массив глубин  (возрастающий или убывающий)

const std::vector<float>& BoreData::GetRadiusCurve(int iRadius)
{
	return m_dataProvider.getData()->vvfDistance[iRadius];
} // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин

const std::vector<float>& BoreData::GetRotation()
{
	return m_dataProvider.getData()->vfRotation;
} // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север

bool BoreData::IsDiameters()
{
	return false;
} // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы
