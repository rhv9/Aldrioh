#pragma once

#define CONSTEXPR_VEC4 constexpr glm::vec4

namespace Colour {

	CONSTEXPR_VEC4 BLACK { 0.0f, 0.0f, 0.0f, 1.0f };
	CONSTEXPR_VEC4 GREY  { 0.5f, 0.5f, 0.5f, 1.0f };
	CONSTEXPR_VEC4 WHITE { 1.0f, 1.0f, 1.0f, 1.0f };
	CONSTEXPR_VEC4 RED   { 1.0f, 0.0f, 0.0f, 1.0f };
	CONSTEXPR_VEC4 BLUE  { 0.0f, 0.0f, 1.0f, 1.0f };
	CONSTEXPR_VEC4 GREEN { 0.0f, 1.0f, 0.0f, 1.0f };



}