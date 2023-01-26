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
	} // ���������� ���-�� ������ ��������

	const std::vector<float>& IBoreData::GetDepths() 
	{ 
		std::vector<float> a; 
		return m_dataProvider.getData()->vfDepth;
	} // ���������� ���������� ������ ������  (������������ ��� ���������)

	const std::vector<float>& IBoreData::GetRadiusCurve(int iRadius) 
	{ 
		return m_dataProvider.getData()->vvfDistance[iRadius];
	} // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������

	const std::vector<float>& IBoreData::GetRotation() 
	{ 
		return m_dataProvider.getData()->vfRotation;
	} // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����

	bool IBoreData::IsDiameters()  
	{ 
		return false; 
	} // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������

}