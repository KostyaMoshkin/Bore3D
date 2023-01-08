// DataProvider.cpp : Определяет экспортируемые функции для DLL.
//

#include "pch.h"
#include "framework.h"
#include "DataProvider.h"

namespace DataProvider
{

    // Конструктор для экспортированного класса.
    CDataProvider::CDataProvider()
    {
        return;
    }

    bool CDataProvider::init(const char* sFileName_)
    {
        return false;
    }
}