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
		IDiaMapper() {}
		~IDiaMapper() {}
		//IDiaMapper* Clone();   // ������� ����� �������.

		double GeoToLP(double x) { return 0; } // ������������� �� ������������� ��������� �  ���������� ������ (x ��� y)
		double LPToGeo(double x) { return 0; } // ������������� �� ���������� ��������� ������ �  �������������

		void SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1) { return; }
		void SetLog(bool bLogScale) { return; } // ����� ��������/��������������� �����
		void Move(double xOffset) { return; } // �������� �������������� �� xOffset ���������� ���������

		// ���������� ��� �������������� 
		// DiaMapper_Linear SF_LIN	// ������� �������� �����
		// DiaMapper_Log SF_LOG	// ������� ��������������� �����
		// DiaMapper_PieceLinear 2	// ������� -�������� �����
		// DiaMapper_Complex 3		   // ������� ����� ��������������
		int GetForm() { return 0; }
	};
}