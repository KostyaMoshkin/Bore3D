#include "pch.h"
#include "IBoreData.h"


namespace DataProvider
{
	IBoreData::IBoreData(const char* sFileName_)
	{
		m_dataProvider.init(sFileName_);
	}

	IBoreData::~IBoreData()
	{

	}

	int IBoreData::GetCurveCount()  
	{ 
		return (int)m_dataProvider.getData()->vfDepth.size();
	} // возвращает кол-во кривых радиусов

	const std::vector<float>& IBoreData::GetDepths() 
	{ 
		std::vector<float> a; 
		return m_dataProvider.getData()->vfDepth;
	} // возвращает монотонный массив глубин  (возрастающий или убывающий)

	const std::vector<float>& IBoreData::GetRadiusCurve(int iRadius) 
	{ 
		return m_dataProvider.getData()->vvfDistance[iRadius];
	} // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин

	const std::vector<float>& IBoreData::GetRotation() 
	{ 
		return m_dataProvider.getData()->vfRotation;
	} // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север

	bool IBoreData::IsDiameters()  
	{ 
		return false; 
	} // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы

}