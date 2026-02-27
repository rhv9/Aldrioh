#pragma once

#ifdef GLOBAL_GR_CPP
#define GLOBAL_GR_EXTERN
#else
#define GLOBAL_GR_EXTERN extern
#endif

#include "Level/Item/ItemRegistry.h"

struct GlobalResource
{
	ItemRegistry itemRegistry;
};

namespace GR {

	GLOBAL_GR_EXTERN std::unique_ptr<GlobalResource> gr;

}