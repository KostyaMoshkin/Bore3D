#include "pch.h"
#include "IDiaMapper.h"

namespace DataProvider
{
	IDiaMapper::IDiaMapper() {}
	IDiaMapper::~IDiaMapper() {}

	double IDiaMapper::GeoToLP(double x) 
	{ 
		return x; 
	}

	double IDiaMapper::LPToGeo(double x) 
	{ 
		return x; 
	}

	void IDiaMapper::SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1) 
	{ 
		return; 
	}

	void IDiaMapper::SetLog(bool bLogScale) 
	{
		return; 
	}
	void IDiaMapper::Move(double xOffset) 
	{ 
		return; 
	}

	int IDiaMapper::GetForm() 
	{ 
		return 0; 
	}

}