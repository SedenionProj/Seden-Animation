#pragma once

inline float secondsToStep(float dt, float sec) {
	return dt / sec;
}

template<typename T>
inline T lerp(T v1, T v2, float t) {
	return v1* (1.f - t) + v2 * t;
}