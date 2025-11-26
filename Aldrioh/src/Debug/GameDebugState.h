#pragma once

#ifdef GAMEDEBUGSTATE_CPP
	#define GLOBAL_VARIABLE(VAR_NAME, INIT) VAR_NAME = INIT
	#define EXTERN_GAMEDEBUGSTATE
#else
	#define GLOBAL_VARIABLE(VAR_NAME, INIT) VAR_NAME
	#define EXTERN_GAMEDEBUGSTATE extern
#endif

// Ok this is code horror but it saves me having to edit two files to add one variable...
// ...Am I too lazy...?

namespace GameDebugState {

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(showCollisionBox, false);

}