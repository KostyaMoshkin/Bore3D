#pragma once

#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif


	// маппер одномерного преобразования физическая координаты <--> экранная координата
	class DATAPROVIDER_API IDiaMapper
	{
	protected:
		float m_fK = 0, m_fX0 = 0;

	public:
		IDiaMapper() = default;
		virtual ~IDiaMapper() {}
		//IDiaMapper* Clone();   // сделать копию маппера.

		virtual float GeoToLP(float x) = 0; // преобазование из геологических координат в  логические экрана (x или y)
		virtual float LPToGeo(float x) = 0; // преобазование из логических координат экрана в  геологические

		virtual void SetGeo0LP0(float Geo0, float LP0) = 0;

		virtual void SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1) = 0;
		virtual void SetLog(bool bLogScale) = 0; // задаёт линейную/логарифмическую шкалу
		virtual void Move(float xOffset) = 0; // сдвигает преобразование на xOffset логических координат

		// возвращает тип преобразования 
		// DiaMapper_Linear SF_LIN	// обычная линейная шкала
		// DiaMapper_Log SF_LOG	// обычная логарифмическая шкала
		// DiaMapper_PieceLinear 2	// кусочно -линейная шкала
		// DiaMapper_Complex 3		   // сложная форма преобразования
		virtual int GetForm() = 0;
	};
