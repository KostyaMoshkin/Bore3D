#pragma once


// маппер одномерного преобразования физическая координаты <--> экранная координата
class IDiaMapper
{
public:
	~IDiaMapper() {};
	//IDiaMapper* Clone();   // сделать копию маппера.

	double GeoToLP(double x) { return 0; } // преобазование из геологических координат в  логические экрана (x или y)
	double LPToGeo(double x) { return 0; } // преобазование из логических координат экрана в  геологические

	void SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1) { return; }
	void SetLog(bool bLogScale) { return; } // задаёт линейную/логарифмическую шкалу
	void Move(double xOffset) { return; } // сдвигает преобразование на xOffset логических координат

	// возвращает тип преобразования 
	// DiaMapper_Linear SF_LIN	// обычная линейная шкала
	// DiaMapper_Log SF_LOG	// обычная логарифмическая шкала
	// DiaMapper_PieceLinear 2	// кусочно -линейная шкала
	// DiaMapper_Complex 3		   // сложная форма преобразования
	int GetForm() { return 0; }
};
