
// Bore3Dtest.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CBore3DtestApp:
// Сведения о реализации этого класса: Bore3Dtest.cpp
//

class CBore3DtestApp : public CWinApp
{
public:
	CBore3DtestApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CBore3DtestApp theApp;
