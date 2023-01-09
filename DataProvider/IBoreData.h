#pragma once

#include "dataprovider.h"

#include <vector>

namespace DataProvider
{

	// ��������� ������������ ���������� ������ ��� ����������� 3D-������ ��������
	// ������ �������������� � ���� �������� �������� �� �������� ���
	// �������� ��� ��������� �������� (���������), ��������������� ���������� ������ ��� ������ ��������
	// �� ������ ������� ����� ����� ������� ���� ������� �������� ������������ ��������� ����������� �� �����
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
		int GetCurveCount() override; // ���������� ���-�� ������ ��������
		const std::vector<float>& GetDepths() override; // ���������� ���������� ������ ������  (������������ ��� ���������)
		const std::vector<float>& GetRadiusCurve(int iRadius) override; // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������
		const std::vector<float>& GetRotation() override; // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����
		bool IsDiameters() override; // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
	};
}