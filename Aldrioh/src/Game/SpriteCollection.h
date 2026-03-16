
#if !defined(SPRITECOLLECTION_GUARD) || defined(SPRITES_CPP)
#define SPRITECOLLECTION_GUARD

#ifdef SPRITES_CPP
#define SPRITES_EXTERN
#else
#define SPRITES_EXTERN extern
#endif

#ifdef SPRITES_VAR_DEFINER
#define SPRITES_MACRO(name, x, y) SPRITES_EXTERN spriteid_t null;
#elif defined(SPRITES_CPP)
#define SPRITES_MACRO(name, x, y) SPRITES_EXTERN spriteid_t null;
#else
#define SPRITES_MACRO(name, x, y) SPRITES_EXTERN spriteid_t name;
#endif

using spriteid_t = int;

class Texture;
class SubTexture;

namespace Sprites {

#ifndef SPRITES_VAR_DEFINER
	void Init();
	constexpr glm::vec2 TileSize{ 16.0f, 16.0f };
	// Textures
	SPRITES_EXTERN Texture* squareTileTexture;
	SubTexture* get(spriteid_t index);
#endif

//#ifdef SPRITES_CPP
//	void InitInternal() {
//#endif

		// Subtextures

		SPRITES_EXTERN spriteid_t null;
		SPRITES_EXTERN spriteid_t shadow;
		SPRITES_EXTERN spriteid_t sand_1;
		SPRITES_EXTERN spriteid_t sand_rock;
		SPRITES_EXTERN spriteid_t sand_cactus;
		SPRITES_EXTERN spriteid_t bullet_fire;
		SPRITES_EXTERN spriteid_t bullet_red;
		SPRITES_EXTERN spriteid_t bullet_white;
		SPRITES_EXTERN spriteid_t coin;
		SPRITES_EXTERN spriteid_t jewel;


		SPRITES_EXTERN spriteid_t player_head;
		SPRITES_EXTERN spriteid_t slime;
		SPRITES_EXTERN spriteid_t target;
		SPRITES_EXTERN spriteid_t spawner;

		SPRITES_EXTERN spriteid_t player_ship;

		SPRITES_EXTERN spriteid_t asteroid_small;
		SPRITES_EXTERN spriteid_t drone_normal;
		SPRITES_EXTERN spriteid_t drone_tank;

		SPRITES_EXTERN spriteid_t borderBox;
		SPRITES_EXTERN spriteid_t greenBox;
		SPRITES_EXTERN spriteid_t square;

//#ifdef SPRITES_CPP
//	}
//#endif

	SPRITES_EXTERN std::vector<int> animPlayerUp, animPlayerDown, animPlayerLeft, animPlayerRight;
	SPRITES_EXTERN std::vector<int> animBossUp, animBossDown, animBossLeft, animBossRight;
}
#endif
