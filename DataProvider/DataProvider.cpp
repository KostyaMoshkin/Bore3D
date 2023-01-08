// DataProvider.cpp : Определяет экспортируемые функции для DLL.
//

#include "pch.h"
#include "framework.h"
#include "DataProvider.h"

namespace DataProvider
{
    static std::vector<std::string> split(std::string sLine_, std::string sDelimiter_)
    {
        std::vector<std::string> sResult;
        size_t pos = 0;
        std::string sToken;
        while ((pos = sLine_.find(sDelimiter_)) != std::string::npos) {
            sToken = sLine_.substr(0, pos);
            sResult.push_back(sToken);
            sLine_.erase(0, pos + sDelimiter_.length());
        }

        return sResult;
    }

    static GeoPoint getGeoPoint(const std::string sLine_)
    {
        std::vector<std::string> sFields = split(sLine_, "\t");

        GeoPoint geoPoint;

        if (sFields.size() < 5)
            return geoPoint;

        for (std::string& sParam : sFields)
        {
            int index = sParam.find(',');

            if (index < 0)
                continue;

            sParam.replace(index, index + 1, ".");
        }

        geoPoint.fDepth = (float)atof(sFields[0].c_str());
        for (int i = 1; i < sFields.size(); ++i)
        {
            geoPoint.fDistance.push_back((float)atof(sFields[i].c_str()));
        }

        return geoPoint;
    }


    // Конструктор для экспортированного класса.
    CDataProvider::CDataProvider()
    {
        return;
    }

    bool CDataProvider::init(const char* sFileName_)
    {
        std::ifstream file(sFileName_);

        if (!file)
            return false;

        char sLine[1024]; // буфер для чтения одной строки

        while (!file.eof())
        {
            file.getline(sLine, 100); // прочитать строку из файла в буфер str
            m_data.push_back(getGeoPoint(sLine));
        }

        file.close();

        return false;
    }
}