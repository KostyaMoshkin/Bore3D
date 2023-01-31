#pragma once
#include <IDiaMapper.h>

class DiaMapper : public IDiaMapper
{

public:
	DiaMapper();
	~DiaMapper();
	//IDiaMapper* Clone();   // ������� ����� �������.

	float GeoToLP(float x) override; // ������������� �� ������������� ��������� �  ���������� ������ (x ��� y)
	float LPToGeo(float x) override; // ������������� �� ���������� ��������� ������ �  �������������

	void SetGeo0LP0(float Geo0, float LP0) override;

	void SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1) override;
	void SetLog(bool bLogScale) override; // ����� ��������/��������������� �����
	void Move(float xOffset) override; // �������� �������������� �� xOffset ���������� ���������

	// ���������� ��� �������������� 
	// DiaMapper_Linear SF_LIN	// ������� �������� �����
	// DiaMapper_Log SF_LOG	// ������� ��������������� �����
	// DiaMapper_PieceLinear 2	// ������� -�������� �����
	// DiaMapper_Complex 3		   // ������� ����� ��������������
	int GetForm() override;
};

