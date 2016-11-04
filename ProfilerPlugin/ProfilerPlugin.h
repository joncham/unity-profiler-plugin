#pragma once

#ifdef _MSC_VER
	#ifdef PROFILERPLUGIN_EXPORTS
		#define PROFILERPLUGIN_API extern "C" __declspec(dllexport) 
	#else
		#define PROFILERPLUGIN_API extern "C" __declspec(dllimport)
	#endif
#else
	#define PROFILERPLUGIN_API
#endif

PROFILERPLUGIN_API int Initialize (void);
