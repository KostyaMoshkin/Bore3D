#include "pch.h"
#include "Render.h"

namespace GL {

    int Render::getVersionGl()
    {
        static int nVersionFull = -1;

        if (nVersionFull != -1)
            return nVersionFull;

        //------------------------------------------------------------------------------------------

        const GLubyte* pVersion = glGetString(GL_VERSION);
        if (!pVersion)
            return nVersionFull;

        std::string sVersion = (const char*)pVersion;
        if (sVersion.length() < 3)
            return nVersionFull;

        int nVersionMain = sVersion[0] - '0';
        int nVersionSub = sVersion[2] - '0';

        nVersionFull = nVersionMain * 10 + nVersionSub;

        return nVersionFull;
    }
}