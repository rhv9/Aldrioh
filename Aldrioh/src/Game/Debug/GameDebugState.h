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

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(shouldUpdateScene, true);
	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(enabledImGui, false);

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(showLvlUpUI, false);

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(showCollisionBox, false);
	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(showCollisionZoneVisualisation, false);

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(renderCollectableCells, false);
	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(renderChunkBordersBeingRendered, false);
	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(showLoadedAndUnloadedCollectableChunks, false);

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(renderLevelArea, false);
	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(enabledDebugCamera, false);

	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(level_spawnEntites, true);
	EXTERN_GAMEDEBUGSTATE bool GLOBAL_VARIABLE(clickToSpawnEnemies, false);


}