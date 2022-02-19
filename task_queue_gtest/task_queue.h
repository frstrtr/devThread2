#pragma once
#ifndef _TASK_QUEUE_
#define _TASK_QUEUE_

#include <thread>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <memory>
#include <functional>

#include "task.h"

class task_queue {
public:
	template<class F, class R = std::result_of_t<F& ()>>
	auto create_task(F&& f) {
		std::packaged_task<R()> p(std::forward<F>(f));
		auto r = std::shared_future<R>(p.get_future());
		{
			std::unique_lock<std::mutex> l(m);
			work.push(std::move(p));
		}
		cv.notify_one();
		return std::make_shared<task<R>>(std::move(r));
	}
	void start_worker() {
		worker_future = std::async(std::launch::async, [this]() { worker(); });
	}
	void stop() {
		if (!active) return;
		std::unique_lock<std::mutex> l(m);
		//чтобы стопнуть поток нужно добавить 
		//пустую таску(удв. условия для cv) затем выполнить её и сделать break из цикла
		work.emplace([]() {});
		active = false;
		cv.notify_one();

	}
private:
	void worker() {
		std::cout << "worker started..." << std::endl;
		while (active) {
			std::packaged_task<void()> f;
			{
				std::unique_lock<std::mutex> l(m);
				cv.wait(l, [&] {return !work.empty(); });
				f = std::move(work.front());
				work.pop();
			}
			f();
		}
		std::cout << "worker ended..." << std::endl;
	}
private:
	std::mutex m;
	std::condition_variable cv;
	std::queue<std::packaged_task<void()>> work;
	std::future<void> worker_future;
	bool active = true;
};


#endif