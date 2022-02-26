# CMakeList.txt : CMake project for task_queue_gtest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (task_queue_gtest    "task_queue.h" "main.cpp" "task.h" "thread_utils.h")

# TODO: Add tests and install targets if needed.
