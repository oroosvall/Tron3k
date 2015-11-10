#ifndef DLLINCLUDE_H
#define DLLINCLUDE_H

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif

#include "IEngine.h"

extern "C" ENGINE_API IEngine* CraeteEningeObject();


#endif