#pragma once

namespace Math {

# define PI           3.14159265358979323846 

	namespace Random {

		uint32_t linearInt(int min, int max);
		float linearFloat(float min, float max);
	}

	float sinRad(float val);
	float cosRad(float val);


	float degreesToRad(float val);

}