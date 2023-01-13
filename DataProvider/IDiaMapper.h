#pragma once

#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif

namespace DataProvider
{

	// маппер одномерного преобразования физическая координаты <--> экранная координата
	class DATAPROVIDER_API IDiaMapper
	{
	public:
		IDiaMapper();
		~IDiaMapper();
		//IDiaMapper* Clone();   // сделать копию маппера.

		double GeoToLP(double x); // преобазование из геологических координат в  логические экрана (x или y)
		double LPToGeo(double x); // преобазование из логических координат экрана в  геологические

		void SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1);
		void SetLog(bool bLogScale); // задаёт линейную/логарифмическую шкалу
		void Move(double xOffset); // сдвигает преобразование на xOffset логических координат

		// возвращает тип преобразования 
		// DiaMapper_Linear SF_LIN	// обычная линейная шкала
		// DiaMapper_Log SF_LOG	// обычная логарифмическая шкала
		// DiaMapper_PieceLinear 2	// кусочно -линейная шкала
		// DiaMapper_Complex 3		   // сложная форма преобразования
		int GetForm();
	};
}