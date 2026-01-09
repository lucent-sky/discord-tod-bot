cmake_minimum_required(VERSION 3.20)
project(truthbot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(dpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

set(DPP_DIR "/home/d/dpp-install")
include_directories(${DPP_DIR}/include)
link_directories(${DPP_DIR}/lib)


add_executable(truthbot
    main.cpp
    commands/add.cpp
    commands/dare.cpp
    commands/random.cpp
    commands/truth.cpp
    utils/json_store.cpp
)

target_link_libraries(truthbot PRIVATE dpp)
