#pragma once
#include <IDiaMapper.h>

class DiaMapper : public IDiaMapper
{

public:
	DiaMapper();
	~DiaMapper();
	//IDiaMapper* Clone();   // сделать копию маппера.

	float GeoToLP(float x) override; // преобазование из геологических координат в  логические экрана (x или y)
	float LPToGeo(float x) override; // преобазование из логических координат экрана в  геологические

	void SetGeo0LP0(float Geo0, float LP0) override;

	void SetGeoRangeLPRange(float Geo0, float Geo1, float LP0, float LP1) override;
	void SetLog(bool bLogScale) override; // задаЄт линейную/логарифмическую шкалу
	void Move(float xOffset) override; // сдвигает преобразование на xOffset логических координат

	// возвращает тип преобразовани€ 
	// DiaMapper_Linear SF_LIN	// обычна€ линейна€ шкала
	// DiaMapper_Log SF_LOG	// обычна€ логарифмическа€ шкала
	// DiaMapper_PieceLinear 2	// кусочно -линейна€ шкала
	// DiaMapper_Complex 3		   // сложна€ форма преобразовани€
	int GetForm() override;
};

