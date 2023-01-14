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
		float m_fK, m_fX0;

	public:
		IDiaMapper();
		~IDiaMapper();
		//IDiaMapper* Clone();   // ������� ����� �������.

		float GeoToLP(float x); // ������������� �� ������������� ��������� �  ���������� ������ (x ��� y)
		float LPToGeo(float x); // ������������� �� ���������� ��������� ������ �  �������������

		void SetGeo0LP0(float Geo0, float LP0);

		void SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1);
		void SetLog(bool bLogScale); // ����� ��������/��������������� �����
		void Move(float xOffset); // �������� �������������� �� xOffset ���������� ���������

		// ���������� ��� �������������� 
		// DiaMapper_Linear SF_LIN	// ������� �������� �����
		// DiaMapper_Log SF_LOG	// ������� ��������������� �����
		// DiaMapper_PieceLinear 2	// ������� -�������� �����
		// DiaMapper_Complex 3		   // ������� ����� ��������������
		int GetForm();
	};
}