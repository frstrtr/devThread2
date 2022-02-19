#include <gtest/gtest.h>
#include "task_queue_gtest/task_queue.h"
#include "task_queue_gtest/thread_utils.h"

/*
* Этот тест проверяет, что поток умеет ждать таску
*/
TEST(WAIT_TEST, BasicAssertions) {
	task_queue t;
	t.start_worker();
	
	int result = 0;
	auto first_task = [&result]() {
		std::cout << "result+=2 and wait 2 sec..." << std::endl;
		result += 2;
		sleep_ms(2000);
		result += 2;
		std::cout << "first task completed" << std::endl;
	};
	auto second_task = [&result]() {
		std::cout << "wait 8 sec and +=2..." << std::endl; 
		sleep_ms(2000); 
		result += 2; 
		std::cout << "second task completed"; 
	};

	t.create_task(first_task)->wait_for_result();
	std::cout << "v1";
	t.create_task(second_task)->wait_for_result();
	std::cout << "v2";
	t.stop();

	EXPECT_EQ(result, 6);
}

/*
* Этот тест проверяет, что потоки и очереди могут выполнять совместную работу
* над ресурсом и мы не упадем от взаимоблокировки
*/
TEST(ACM_TEST, BasicAssertions) {
	task_queue t;
	task_queue t2;

	t2.start_worker();
	t.start_worker();

	int result = 0;
	bool completed = false;

	t2.create_task([&completed, &result]() {
		while (!completed) {
			sleep_ms(300);
			std::cout << "current result=" << result << std::endl;
		}
	});

	for (int i = 0; i < 50; i++) {
		t.create_task([&result]() {
			//эмитация долгих вычислений
			sleep_ms(150);
			result += 2; 
		});
	}

	t.create_task([&result, &completed]() {
		completed = true;
		std::cout << "result=" << result << std::endl;
	});

	t.create_task([]() {})->wait_for_result();
	t.stop();
	t2.stop();

	EXPECT_EQ(result, 100);
}
