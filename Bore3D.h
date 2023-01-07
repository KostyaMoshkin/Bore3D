#pragma once

// интерфейс абстрактного поставщика данных для отображения 3D-ствола скважины
// данные представляются в виде массивов значений по глубинам для
// радиусов или удвоенных радиусов (диаметров), располагающихся равномерно вокруг оси ствола скважины
// на каждой глубине также задан поворот всей системы радиусов относительно условного направления на север
struct IBoreData
{
	virtual int GetCurveCount() const = 0; // возвращает кол-во кривых радиусов
	virtual const std::vector<float>& GetDepths() = 0; // возвращает монотонный массив глубин  (возрастающий или убывающий)
	virtual const std::vector<float>& GetRadiusCurve(int iRadius) = 0; // возвращает массив значений i-го радиуса по глубинам, соответствующий массиву глубин
	virtual const std::vector<float>& GetRotation() = 0; // возвращает массив углов (в градусах) поворота первого радиуса относительно направления на север
	virtual bool IsDiameters() const = 0; // возвращает true, если кривые представляют из себя удвоенные радиусы, иначе просто радиусы
};


// маппер одномерного преобразования физическая координаты <--> экранная координата
interface IDiaMapper
{
	virtual ~IDiaMapper() {};
	virtual IDiaMapper* Clone() = 0;   // сделать копию маппера.

	virtual double GeoToLP(double x) = 0; // преобазование из геологических координат в  логические экрана (x или y)
	virtual double LPToGeo(double x) = 0; // преобазование из логических координат экрана в  геологические

	virtual void SetGeoRangeLPRange(double Geo0, double Geo1, double LP0, double LP1) = 0;
	virtual void SetLog(BOOL bLogScale) = 0; // задаёт линейную/логарифмическую шкалу
	virtual void Move(double xOffset) = 0; // сдвигает преобразование на xOffset логических координат

// возвращает тип преобразования 
// DiaMapper_Linear SF_LIN	// обычная линейная шкала
// DiaMapper_Log SF_LOG	// обычная логарифмическая шкала
// DiaMapper_PieceLinear 2	// кусочно -линейная шкала
// DiaMapper_Complex 3		   // сложная форма преобразования
	virtual int GetForm() = 0;
};

interface IBore3D
{
	// прототип метода для отображения части 3D-ствола скважины в bitmap
	// 
	bool InitBore3D(
		IBoreData* pData, // интерфейс доступа к данным развёртки
		float fLogPerPixel // коэффициент соотношения между логическими единицами (используются маппером) и пикселями экрана
	);

	bool InitWindow(
		HWND hWnd, // хэндл скрытого окна для проведения теневых отрисовок в OpenGL (если надо)
		int nWidth,
		int nHeight
	);

	// палитра цветов, "натягиваемая" на диапозон значений радиусов fMinRadius, fMaxRadius);
	bool InitPalette(const std::vector<COLORREF>& vecPalette);

	int GetBitmap(
		CBitmap& bmp,
		const RECT* pVisualRect, // прямоугольник в логических единицах отображающий часть 3D-ствола (top,bottom соответствует fTop,fBottom при преобразовании в pMapper)
		IDiaMapper* pMapper, // отображение глубина <--> логические единицы по вертикали (не путать с пикселями)
		float fTop, float fBottom, // интервал глубин (окно) отображения ствола скважины
		float fRotation, // дополнительный угол поворота всего ствола вокруг своей оси
		// совокупно следующие 4 параметра определяют шкалу для отображения ридиусов (как значение радиуса преобразуется в видимую толщину ствола)
		float fMinRadius, float fMaxRadius, // минимальное и максимальное значение радиуса/диаметра, соответствующее ширине 
		int nMinRadiusLP, int nMaxRadiusLP, // видимый размер в логических единицах для минимального и максимального радиуса 
		float fIsometryAngle, // угол изометрической проекции
		bool bDrawMesh
	);
};

