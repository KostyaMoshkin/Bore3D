﻿// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

#include "framework.h"


#include <utility>
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>

#include <glew.h>

extern HMODULE g_hModule;
extern HMODULE g_hResource;

#endif //PCH_H
