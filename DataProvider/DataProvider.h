#pragma once

// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа DATAPROVIDER_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции DATAPROVIDER_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef DATAPROVIDER_EXPORTS
#define DATAPROVIDER_API __declspec(dllexport)
#else
#define DATAPROVIDER_API __declspec(dllimport)
#endif

#include <vector>

namespace DataProvider
{
	struct GeoPoint
	{
		std::vector<float> vfDepth;
		std::vector<std::vector<float>> vvfDistance;
		std::vector<float> vfRotation;
	};

	// Этот класс экспортирован из библиотеки DLL
	class DATAPROVIDER_API CDataProvider
	{
		GeoPoint m_data; ///<  Основной массив с данными

	public:
		CDataProvider(void);

	public:
		bool init(const char* sFileName_);

		GeoPoint* getData() { return &m_data; }
	};
}