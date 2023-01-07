#pragma once

#include "ControlGL.h"

#include "IBoreData.h"
#include "IDiaMapper.h"

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
		float m_fGreen = 0;

	public:
		BoreControl();

	public:
		// ������������ ����� ControlGL
		bool paint() override;


	public:
		// �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
		// 
		bool InitBore3D(
			IBoreData* pData, // ��������� ������� � ������ ��������
			float fLogPerPixel // ����������� ����������� ����� ����������� ��������� (������������ ��������) � ��������� ������
		);

		//bool InitWindow(
		//	HWND hWnd, // ����� �������� ���� ��� ���������� ������� ��������� � OpenGL (���� ����)
		//	int nWidth,
		//	int nHeight
		//);

		// ������� ������, "������������" �� �������� �������� �������� fMinRadius, fMaxRadius);
		bool InitPalette(const std::vector<COLORREF>& vecPalette);

		int GetBitmap(
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

}