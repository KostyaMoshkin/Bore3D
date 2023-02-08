#pragma once
#include <IBoreData.h>

#include <dataprovider.h>

class BoreData : public IBoreData
{
	DataProvider::CDataProvider m_dataProvider;

public:
	BoreData() {}
	BoreData(const char* sFileName_);
	virtual ~BoreData();

public:
	int GetCurveCount() override; // ���������� ���-�� ������ ��������
	const std::vector<float>& GetDepths() override; // ���������� ���������� ������ ������  (������������ ��� ���������)
	const std::vector<float>& GetRadiusCurve(int iRadius) override; // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������
	const std::vector<float>& GetRotation() override; // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����
	bool IsDiameters() override; // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
};
