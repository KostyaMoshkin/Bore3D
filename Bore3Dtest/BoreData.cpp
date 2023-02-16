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
} // ���������� ���-�� ������ ��������

const std::vector<float>& BoreData::GetDepths()
{
	std::vector<float> a;
	return m_dataProvider.getData()->vfDepth;
} // ���������� ���������� ������ ������  (������������ ��� ���������)

const std::vector<float>& BoreData::GetRadiusCurve(int iRadius)
{
	return m_dataProvider.getData()->vvfDistance[iRadius];
} // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������

const std::vector<float>& BoreData::GetRotation()
{
	return m_dataProvider.getData()->vfRotation;
} // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����

bool BoreData::IsDiameters()
{
	return false;
} // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
