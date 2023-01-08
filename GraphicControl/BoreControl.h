#pragma once
#include "pch.h"

#include "ControlGL.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <array>

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
		float m_fGreen = 0;

		std::array<float, 3> m_vBkgColor;

		bool m_bDataInit = false;
		bool m_bPaletteInit = false;

	public:
		BoreControl();

	public:
		// ������������ ����� ControlGL
		void paint() override;

	public:
		void setBkgColor(float r_, float g_, float b_);

	public:
		// �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
		// 
		bool InitBore3D(
			DataProvider::IBoreData* pData, // ��������� ������� � ������ ��������
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
			DataProvider::IDiaMapper* pMapper, // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
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