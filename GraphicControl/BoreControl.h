#pragma once
#include "pch.h"

#include "ControlGL.h"
#include "RenderBoreSurface.h"

#include <array>
#include <vector>

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
		bool m_bDataInit = false;
		bool m_bPaletteInit = false;

		GL::RenderBoreSurfacePtr m_pRenderBoreSurface;

	public:
		BoreControl();
		~BoreControl();

	public:
		// ������������ ����� ControlGL
		void paint() override;

	public:
		// �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
		// 
		bool InitBore3D(
			void* pData, // ��������� ������� � ������ ��������
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
			void* pMapper, // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
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