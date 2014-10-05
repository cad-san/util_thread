# プロジェクト名
project (sample_ipc)

# CMakeバージョン
cmake_minimum_required (VERSION 2.6)

# ビルド用オプション
set(CMAKE_C_FLAGS "-O4 -Wall")
set(CMAKE_CXX_FLAGS "-O4 -Wall -std=c++11")

# ディレクトリ指定
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(INC_DIR  ${ROOT_DIR}/include)
set(LIB_DIR  ${ROOT_DIR}/lib)

# ライブラリ
set(LIBS util_thread pthread rt)

include_directories(${INC_DIR})
link_directories(${LIB_DIR})

add_executable (client client.cpp)
add_executable (server server.cpp)

target_link_libraries(client ${LIBS})
target_link_libraries(server ${LIBS})
