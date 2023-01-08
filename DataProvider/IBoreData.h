#pragma once

#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif

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
		int GetCurveCount() const { return 0; } // ���������� ���-�� ������ ��������
		const std::vector<float>& GetDepths() { std::vector<float> a; return a; } // ���������� ���������� ������ ������  (������������ ��� ���������)
		const std::vector<float>& GetRadiusCurve(int iRadius) { std::vector<float> a; return a; } // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������
		const std::vector<float>& GetRotation() { std::vector<float> a; return a; } // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����
		bool IsDiameters() const { return true; } // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
	};
}