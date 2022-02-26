#ifndef _THREAD_UTILS_
#define _THREAD_UTILS_

inline void sleep_ms(long ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#endif