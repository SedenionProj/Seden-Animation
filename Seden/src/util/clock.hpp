#pragma once

namespace Seden {
	class Clock {
	public:
		using T_Clock = std::chrono::high_resolution_clock;

		Clock()
			: m_start_time(T_Clock::now()) {
		}

		float getElapsedTime() const {
			// in second
			auto now = T_Clock::now();
			std::chrono::duration<float> elapsed_time = now - m_start_time;
			return elapsed_time.count();
		}

		float getElapsedTimeAndReset() {
			// in second
			auto now = T_Clock::now();
			std::chrono::duration<float> elapsed_time = now - m_start_time;
			reset();
			return elapsed_time.count();
		}

		void reset() {
			m_start_time = T_Clock::now();
		}

	private:
		T_Clock::time_point m_start_time;
	};

}