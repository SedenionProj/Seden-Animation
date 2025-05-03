#pragma once

namespace Seden {
	class Sync {
	public:
		Sync() : m_blocked(false) {}

		void block() {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_blocked = true;
		}

		void unBlock() {
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_blocked = false;
			}
			m_cv.notify_all();
		}

		void waitUntilUnblocked() {
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this] { return !m_blocked; });
		}

		bool isBlocked() const {
			return m_blocked;
		}

	private:
		std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_blocked;
	};

}