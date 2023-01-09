#pragma once
#include "pch.h"

#include "ControlGL.h"
#include "RenderBoreSurface.h"

#include <IBoreData.h>
#include <IDiaMapper.h>

#include <array>
#include <vector>

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
		bool m_bDataInit = false;
		bool m_bPaletteInit = false;

		GL::RenderBoreSurfacePtr m_pRenderBoreSurface;

		DataProvider::IBoreData* m_pData;

	public:
		BoreControl();

	public:
		// ������������ ����� ControlGL
		void paint() override;

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