cmake_minimum_required (VERSION 3.12)

project (main)

add_executable (main "main.cpp" "buffer_manager.cpp" "storage_manager.cpp" "lru.cpp" "include/lru.h" "include/buffer_control.h" "include/buffer_manager.h" "include/storage_manager.h")