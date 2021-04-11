cmake_minimum_required(VERSION 3.19)
project(5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14 -fno-omit-frame-pointer")

set(CMAKE_SKIP_BUILD_RPATH true)
find_package(Protobuf REQUIRED)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/include)
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl/lib/)
find_package(OpenSSL REQUIRED)

message("Protobuf_LIBRARIES: " ${Protobuf_LIBRARIES})

include_directories(${Protobuf_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include /usr/local/include/grpc)
link_libraries(${Protobuf_LIBRARIES} gRPC::grpc++ gRPC::grpc++_reflection)

add_executable(server src/server.cpp src/helloworld.grpc.pb.cc src/helloworld.pb.cc)

add_executable(client src/client.cpp src/helloworld.grpc.pb.cc src/helloworld.pb.cc)