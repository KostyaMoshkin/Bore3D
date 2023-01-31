#pragma once

#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif


	// ������ ����������� �������������� ���������� ���������� <--> �������� ����������
	class DATAPROVIDER_API IDiaMapper
	{
	protected:
		float m_fK = 0, m_fX0 = 0;

	public:
		IDiaMapper() = default;
		virtual ~IDiaMapper() {}
		//IDiaMapper* Clone();   // ������� ����� �������.

		virtual float GeoToLP(float x) = 0; // ������������� �� ������������� ��������� �  ���������� ������ (x ��� y)
		virtual float LPToGeo(float x) = 0; // ������������� �� ���������� ��������� ������ �  �������������

		virtual void SetGeo0LP0(float Geo0, float LP0) = 0;

		virtual void SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1) = 0;
		virtual void SetLog(bool bLogScale) = 0; // ����� ��������/��������������� �����
		virtual void Move(float xOffset) = 0; // �������� �������������� �� xOffset ���������� ���������

		// ���������� ��� �������������� 
		// DiaMapper_Linear SF_LIN	// ������� �������� �����
		// DiaMapper_Log SF_LOG	// ������� ��������������� �����
		// DiaMapper_PieceLinear 2	// ������� -�������� �����
		// DiaMapper_Complex 3		   // ������� ����� ��������������
		virtual int GetForm() = 0;
	};
