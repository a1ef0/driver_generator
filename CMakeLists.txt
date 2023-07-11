cmake_minimum_required(VERSION 3.13)

project(driver_generator VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
find_program(_PROTOBUF_PROTOC protoc)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Add grpc lib
add_subdirectory(grpc)

find_library(NETSNMP "netsnmp")

add_executable(driver snmp/SNMP_client.h snmp/SNMP_client.cc $<TARGET_OBJECTS:key_value_service_client>)
target_include_directories(driver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/snmp ${_PROTOBUF_INCLUDES})
add_dependencies(driver key_value_service_client)
target_sources(driver PRIVATE main.cc)
target_link_libraries(driver PUBLIC ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} ${NETSNMP})
