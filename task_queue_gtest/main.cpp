#include "task_queue.h"
#include "thread_utils.h"

void main_emulated() {
	while (true) {
		std::cout << "main thread started" << std::endl;
		sleep_ms(5000);

		task_queue t;
		t.start_worker();

		std::cout << "creating tasks..." << std::endl;
		t.create_task(([]() {std::cout << "1" << std::endl; sleep_ms(2000); }));
		t.create_task(([]() {std::cout << "2" << std::endl; sleep_ms(2000); }));
		t.create_task(([]() {std::cout << "3" << std::endl; sleep_ms(2000); }));
		t.create_task(([]() {std::cout << "4" << std::endl; sleep_ms(2000); }));
		std::cout << "end of creating tasks" << std::endl;
		break;
	}
}

int main() {
	std::thread main_thread(main_emulated);
	main_thread.join();
}