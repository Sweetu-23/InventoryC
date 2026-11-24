cmake_minimum_required(VERSION 3.16)
project(inventory_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(inventory_core
    src/models.cpp
    src/repository.cpp
    src/inventory_service.cpp
    src/product_controller.cpp
    src/stock_controller.cpp
    src/order_controller.cpp
    src/router.cpp
    src/util.cpp)
target_include_directories(inventory_core PUBLIC src)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(inventory_core PUBLIC -Wall -Wextra)
endif()

add_executable(inventory_app src/main.cpp)
target_link_libraries(inventory_app inventory_core)

enable_testing()
add_executable(inventory_tests tests/test_inventory.cpp)
target_link_libraries(inventory_tests inventory_core)
add_test(NAME inventory_tests COMMAND inventory_tests)
