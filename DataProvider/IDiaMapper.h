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
		float m_fK, m_fX0;

	public:
		IDiaMapper();
		~IDiaMapper();
		//IDiaMapper* Clone();   // сделать копию маппера.

		float GeoToLP(float x); // преобазование из геологических координат в  логические экрана (x или y)
		float LPToGeo(float x); // преобазование из логических координат экрана в  геологические

		void SetGeo0LP0(float Geo0, float LP0);

		void SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1);
		void SetLog(bool bLogScale); // задаёт линейную/логарифмическую шкалу
		void Move(float xOffset); // сдвигает преобразование на xOffset логических координат

		// возвращает тип преобразования 
		// DiaMapper_Linear SF_LIN	// обычная линейная шкала
		// DiaMapper_Log SF_LOG	// обычная логарифмическая шкала
		// DiaMapper_PieceLinear 2	// кусочно -линейная шкала
		// DiaMapper_Complex 3		   // сложная форма преобразования
		int GetForm();
	};
}