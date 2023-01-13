#pragma once

#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif

namespace DataProvider
{

	// ������ ����������� �������������� ���������� ���������� <--> �������� ����������
	class DATAPROVIDER_API IDiaMapper
	{
	public:
		IDiaMapper();
		~IDiaMapper();
		//IDiaMapper* Clone();   // ������� ����� �������.

		double GeoToLP(double x); // ������������� �� ������������� ��������� �  ���������� ������ (x ��� y)
		double LPToGeo(double x); // ������������� �� ���������� ��������� ������ �  �������������

		void SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1);
		void SetLog(bool bLogScale); // ����� ��������/��������������� �����
		void Move(double xOffset); // �������� �������������� �� xOffset ���������� ���������

		// ���������� ��� �������������� 
		// DiaMapper_Linear SF_LIN	// ������� �������� �����
		// DiaMapper_Log SF_LOG	// ������� ��������������� �����
		// DiaMapper_PieceLinear 2	// ������� -�������� �����
		// DiaMapper_Complex 3		   // ������� ����� ��������������
		int GetForm();
	};
}