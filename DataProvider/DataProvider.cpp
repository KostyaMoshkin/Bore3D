#include "pch.h"
#include "framework.h"
#include "DataProvider.h"

#include <algorithm>

namespace DataProvider
{
    static std::vector<std::string> split(std::string sLine_, std::string sDelimiter_)
    {
        std::vector<std::string> sResult;

        size_t pos = 0;
        std::string sToken;

        while ((pos = sLine_.find(sDelimiter_)) != std::string::npos) 
        {
            sToken = sLine_.substr(0, pos);
            sResult.push_back(sToken);
            sLine_.erase(0, pos + sDelimiter_.length());
        }

        sResult.push_back(sLine_);

        return sResult;
    }

    static std::vector<std::string> getStringValues(const std::string& sLine_)
    {
        std::vector<std::string> sFields = split(sLine_, "\t");

        GeoPoint geoPoint;

        if (sFields.size() < 5)
            return std::vector<std::string>(0);

        for (std::string& sParam : sFields)
        {
            int index = (int)sParam.find(',');

            if (index < 0)
                continue;

            sParam.replace(index, index, ".");
        }

        return sFields;
    }

    static void addGeoPoint(GeoPoint& geoPoint_, std::string& sLine_)
    {
        std::vector<std::string> sFields = getStringValues(sLine_);

        int nCurveCount = (int)sFields.size();

        if (nCurveCount < 5)
            return;

        for (const std::string& value : sFields)
            if (value.empty())
                return;

        geoPoint_.vfDepth.push_back((float)atof(sFields[0].c_str()));

        std::vector<float> vDistance;
        for (size_t i = 1; i < nCurveCount; ++i)
            vDistance.push_back((float)atof(sFields[i].c_str()));

        geoPoint_.vvfDistance.push_back(vDistance);

        float fRandomRotaion = 1.0f * std::rand() / RAND_MAX * 1.0f;

        geoPoint_.vfRotation.push_back(fRandomRotaion);
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

        std::string sLine;

        int i = 0;

        while (std::getline(file, sLine))
        {
            //if (++i > 300)
            //    continue;
            addGeoPoint(m_data, sLine);
        }

        file.close();

        std::sort(m_data.vfDepth.begin(), m_data.vfDepth.end(), [](float a, float b) { return a < b; });

        return true;
    }
}