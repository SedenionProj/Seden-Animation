#pragma once

namespace Seden {
	constexpr float PI = 3.1415927f;

	inline float secondsToStep(float dt, float sec) {
		return dt / sec;
	}

	template<typename T>
	inline T lerp(T v1, T v2, float t) {
		return v1 * (1.f - t) + v2 * t;
	}

	inline uint32_t lerp(uint32_t v1, uint32_t v2, float t) {
		double result = static_cast<double>(v1) * (1.0 - t) + static_cast<double>(v2) * t;
		return static_cast<uint32_t>(result);
	}
}