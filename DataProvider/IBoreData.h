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
		CDataProvider m_dataProvider;

	public:
		IBoreData(const char* sFileName_);
		virtual ~IBoreData();

	public:
		int GetCurveCount(); // ���������� ���-�� ������ ��������
		const std::vector<float>& GetDepths(); // ���������� ���������� ������ ������  (������������ ��� ���������)
		const std::vector<float>& GetRadiusCurve(int iRadius); // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������
		const std::vector<float>& GetRotation(); // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����
		bool IsDiameters(); // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
	};
}