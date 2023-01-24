#pragma once
#include "pch.h"

#include "ControlGL.h"

#include <array>
#include <vector>
#include <memory>

namespace GraphicControl
{

	class GRAPHICCONTROL_API BoreControl : public ControlGL
	{
	public:
		BoreControl();
		~BoreControl();

	public:
		// ������������ ����� ControlGL
		void paint() override;
		bool init() override;

	public:
		// �������� ������ ��� ����������� ����� 3D-������ �������� � bitmap
		// 
		bool InitBore3D(
			void* pData, // ��������� ������� � ������ ��������
			float fLogPerPixel // ����������� ����������� ����� ����������� ��������� (������������ ��������) � ��������� ������
		);

		void InitDiaMapper(
			void* pMapper_ // ����������� ������� <--> ���������� ������� �� ��������� (�� ������ � ���������)
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
			float fTop, float fBottom, // �������� ������ (����) ����������� ������ ��������
			float fRotation, // �������������� ���� �������� ����� ������ ������ ����� ���
			// ��������� ��������� 4 ��������� ���������� ����� ��� ����������� �������� (��� �������� ������� ������������� � ������� ������� ������)
			float fMinRadius, float fMaxRadius, // ����������� � ������������ �������� �������/��������, ��������������� ������ 
			int nMinRadiusLP, int nMaxRadiusLP, // ������� ������ � ���������� �������� ��� ������������ � ������������� ������� 
			float fIsometryAngle, // ���� �������������� ��������
			bool bDrawMesh
		);

	public:
		bool setBkgColor(float r_, float g_, float b_);
		bool setMesColor(float r_, float g_, float b_);
		bool setZeroLineColor(float r_, float g_, float b_, int nWidth_ = 1);

	private:
		struct Implementation;
		std::shared_ptr<Implementation> m_pImpl;
	};

}