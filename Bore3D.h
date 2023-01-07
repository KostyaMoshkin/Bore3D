#pragma once

// ��������� ������������ ���������� ������ ��� ����������� 3D-������ ��������
// ������ �������������� � ���� �������� �������� �� �������� ���
// �������� ��� ��������� �������� (���������), ��������������� ���������� ������ ��� ������ ��������
// �� ������ ������� ����� ����� ������� ���� ������� �������� ������������ ��������� ����������� �� �����
struct IBoreData
{
	virtual int GetCurveCount() const = 0; // ���������� ���-�� ������ ��������
	virtual const std::vector<float>& GetDepths() = 0; // ���������� ���������� ������ ������  (������������ ��� ���������)
	virtual const std::vector<float>& GetRadiusCurve(int iRadius) = 0; // ���������� ������ �������� i-�� ������� �� ��������, ��������������� ������� ������
	virtual const std::vector<float>& GetRotation() = 0; // ���������� ������ ����� (� ��������) �������� ������� ������� ������������ ����������� �� �����
	virtual bool IsDiameters() const = 0; // ���������� true, ���� ������ ������������ �� ���� ��������� �������, ����� ������ �������
};


// ������ ����������� �������������� ���������� ���������� <--> �������� ����������
interface IDiaMapper
{
	virtual ~IDiaMapper() {};
	virtual IDiaMapper* Clone() = 0;   // ������� ����� �������.

	virtual double GeoToLP(double x) = 0; // ������������� �� ������������� ��������� �  ���������� ������ (x ��� y)
	virtual double LPToGeo(double x) = 0; // ������������� �� ���������� ��������� ������ �  �������������

	virtual void SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1) = 0;
	virtual void SetLog(BOOL bLogScale) = 0; // ����� ��������/��������������� �����
	virtual void Move(double xOffset) = 0; // �������� �������������� �� xOffset ���������� ���������

// ���������� ��� �������������� 
// DiaMapper_Linear SF_LIN	// ������� �������� �����
// DiaMapper_Log SF_LOG	// ������� ��������������� �����
// DiaMapper_PieceLinear 2	// ������� -�������� �����
// DiaMapper_Complex 3		   // ������� ����� ��������������
	virtual int GetForm() = 0;
};

interface IBore3D
{
	// �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
	// 
	bool InitBore3D(
		IBoreData* pData, // ��������� ������� � ������ ��������
		float fLogPerPixel // ����������� ����������� ����� ����������� ��������� (������������ ��������) � ��������� ������
	);

	bool InitWindow(
		HWND hWnd, // ����� �������� ���� ��� ���������� ������� ��������� � OpenGL (���� ����)
		int nWidth,
		int nHeight
	);

	// ������� ������, "������������" �� �������� �������� �������� fMinRadius, fMaxRadius);
	bool InitPalette(const std::vector<COLORREF>& vecPalette);

	int GetBitmap(
		CBitmap& bmp,
		const RECT* pVisualRect, // ������������� � ���������� �������� ������������ ����� 3D-������ (top,bottom ������������� fTop,fBottom ��� �������������� � pMapper)
		IDiaMapper* pMapper, // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
		float fTop, float fBottom, // �������� ������ (����) ����������� ������ ��������
		float fRotation, // �������������� ���� �������� ����� ������ ������ ����� ���
		// ��������� ��������� 4 ��������� ���������� ����� ��� ����������� �������� (��� �������� ������� ������������� � ������� ������� ������)
		float fMinRadius, float fMaxRadius, // ����������� � ������������ �������� �������/��������, ��������������� ������ 
		int nMinRadiusLP, int nMaxRadiusLP, // ������� ������ � ���������� �������� ��� ������������ � ������������� ������� 
		float fIsometryAngle, // ���� �������������� ��������
		bool bDrawMesh
	);
};

