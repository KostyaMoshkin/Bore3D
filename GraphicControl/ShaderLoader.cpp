#include "pch.h"

#include <streambuf>

#include "ShaderLoader.h"

#include "resource.h"

#include <WinUser.h>

namespace shader {

	static HMODULE GCM()
	{
		HMODULE hModule = nullptr;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GCM, &hModule);

		return hModule;
	}

	std::string ShaderLoader::get_ShaderBody(int nhaderResourceID_)
	{

		HMODULE hModule = GCM();

		if (hModule == nullptr)
			return std::string("");

		HRSRC rc = ::FindResource(hModule, MAKEINTRESOURCEW(nhaderResourceID_), MAKEINTRESOURCEW(TEXTFILE));

		if (rc == nullptr)
			return std::string("");

		HGLOBAL rcData = ::LoadResource(hModule, rc);

		if (rcData == nullptr)
			return std::string("");

		const char * cShaderBody = static_cast<const char*>(::LockResource(rcData));

		std::string sSahderBody(cShaderBody);

		return sSahderBody;
	}

}
