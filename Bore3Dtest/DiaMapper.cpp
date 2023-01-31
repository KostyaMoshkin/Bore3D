#include "pch.h"
#include "DiaMapper.h"

DiaMapper::DiaMapper() {}
DiaMapper::~DiaMapper() {}

float DiaMapper::GeoToLP(float x)
{
	return x * m_fK + m_fX0;
}

float DiaMapper::LPToGeo(float x)
{
	return x;
}

void DiaMapper::SetGeo0LP0(float Geo0, float LP0)
{
	m_fX0 = -(float)log10(Geo0) * m_fK + LP0;
}

void DiaMapper::SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1)
{
	m_fK = (LP1 - LP0) / (float)(log10(Geo1) - log10(Geo0));
	SetGeo0LP0(Geo0, LP0);
}

void DiaMapper::SetLog(bool bLogScale)
{
	return;
}
void DiaMapper::Move(float xOffset)
{
	return;
}

int DiaMapper::GetForm()
{
	return 0;
}
