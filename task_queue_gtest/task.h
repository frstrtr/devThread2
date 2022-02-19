#ifndef _TASK_
#define _TASK_

#include <future>
#include <chrono>

template<class R>
class task {
public:
	task(std::shared_future<R>&& f) {
		this->future = { f };
	}
	auto wait_for_result() {
		while (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
		return future.get();
	}
private:
	std::shared_future<R> future;
};

#endif