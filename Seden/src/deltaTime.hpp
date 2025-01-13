#pragma once
#include <chrono>

class DeltaTime {
public:
	using Clock = std::chrono::high_resolution_clock;

	DeltaTime()
		: m_start_time(Clock::now()) {
	}

	float getElapsedTime() const {
		// in second
		auto now = Clock::now();
		std::chrono::duration<float> elapsed_time = now - m_start_time;
		return elapsed_time.count();
	}

	void reset() {
		m_start_time = Clock::now();
	}

private:
	Clock::time_point m_start_time;
};
