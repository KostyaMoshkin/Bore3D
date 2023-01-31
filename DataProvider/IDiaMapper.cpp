#include "pch.h"
#include "IDiaMapper.h"

namespace DataProvider
{
	//IDiaMapper::IDiaMapper() {}
	//IDiaMapper::~IDiaMapper() {}

	//float IDiaMapper::GeoToLP(float x)
	//{
	//	return x * m_fK + m_fX0;
	//}

	//float IDiaMapper::LPToGeo(float x)
	//{ 
	//	return x;
	//}

	//void IDiaMapper::SetGeo0LP0(float Geo0, float LP0)
	//{
	//	m_fX0 = -log10(Geo0) * m_fK + LP0;
	//}

	//void IDiaMapper::SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1)
	//{ 
	//	m_fK = (LP1 - LP0) / (log10(Geo1) - log10(Geo0));
	//	SetGeo0LP0(Geo0, LP0);
	//}

	//void IDiaMapper::SetLog(bool bLogScale) 
	//{
	//	return; 
	//}
	//void IDiaMapper::Move(float xOffset)
	//{ 
	//	return; 
	//}

	//int IDiaMapper::GetForm() 
	//{ 
	//	return 0; 
	//}

}
/*

CDiaMapper::CDiaMapper(void)
{
	m_fK=1.;
	m_fX0=0;
	//m_bVertical=TRUE;
	m_bLog=FALSE;
}

CDiaMapper::CDiaMapper(const CDiaMapper &mapper)
{
	m_fK=mapper.m_fK;
	m_fX0=mapper.m_fX0;
	m_bLog=mapper.m_bLog;
}
CDiaMapper::~CDiaMapper(void)
{
}

IDiaMapper *CDiaMapper::Clone()
{
	return new CDiaMapper( *this );
}

double CDiaMapper::GeoToLP(double x) // преобазование из геологических координат в  логические экрана (x или y)
{
	if (m_bLog && x <= 0.) return -9999.; // если в логарифмический маппер передали что-то <=0, то выдаём -9999, чтобы обозначить, что данный фрагмент не должен выводиться
	return ( !m_bLog ) ? (x*m_fK + m_fX0) : (log10(x)*m_fK + m_fX0);
}

double CDiaMapper::LPToGeo(double x) // преобразование из логических координат экрана в  геологические
{
	ASSERT(m_fK != 0.);
	return (!m_bLog) ? ((x - m_fX0) / m_fK) : (exp(log(10.) * (x - m_fX0) / m_fK));
}

void CDiaMapper::Move( double xOffset )
{
	m_fX0 += xOffset;
}

int CDiaMapper::GetForm()
{
	return m_bLog ? DiaMapper_Log : DiaMapper_Linear;
}

void CDiaMapper::SetGeo0LP0(double Geo0, double LP0)
{
	if (!m_bLog)
		m_fX0=-Geo0*m_fK+LP0;
	else
		m_fX0=-log10(Geo0)*m_fK+LP0;

}

void CDiaMapper::SetGeoRangeLPRange(double Geo0,double Geo1,double LP0, double LP1)
{
	if (!m_bLog)
	{
		m_fK=(LP1-LP0)/(Geo1-Geo0);
		SetGeo0LP0(Geo0,LP0);
	}
	else
	{
		m_fK=(LP1-LP0)/(log10(Geo1)-log10(Geo0));
		SetGeo0LP0(Geo0,LP0);
	};
}

void init()
{
	CDiaMapper mapper;
	int height = height_pixels(окно в пикс) / fPixelsPerLP;
	mapper.SetGeoRangeLPRange(Top(из глубины), Bottom(из глубины), 0, height);
}

*/