cmake_minimum_required(VERSION 3.16)

project(erts-a1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(
    SystemCLanguage CONFIG REQUIRED
    HINTS "$ENV{SYSTEM_C_PATH}" # Add path to env var
)

add_subdirectory(3.1)
add_subdirectory(3.2)
add_subdirectory(3.3.1)
add_subdirectory(3.3.2)
add_subdirectory(3.4)






