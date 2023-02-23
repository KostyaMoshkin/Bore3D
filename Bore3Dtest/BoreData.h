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

	int GetCurveCount() override; // ���������� ���-�� ������ ��������
	const std::vector<float>& GetDepths() override; // ���������� ���������� ������ ������  (������������ ��� ���������)
	const std::vector<float>& GetRadiusCurve(int iRadius) override; // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������
	const std::vector<float>& GetRotation() override; // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����
	bool IsDiameters() override; // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
};

