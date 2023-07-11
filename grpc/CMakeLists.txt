# Minimum CMake required
cmake_minimum_required(VERSION 3.13)

project(driver VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(proto_name data_transfer_api)

# Get proto files
get_filename_component(proto "../protos/${proto_name}.proto" ABSOLUTE)
get_filename_component(proto_dir "${proto}" DIRECTORY)

# Generate source files
set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/${proto_name}.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${proto_name}.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/${proto_name}.grpc.pb.cc" key_value_service_client.cc)
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${proto_name}.grpc.pb.h")
add_custom_command(
    OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${proto_dir}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${proto}"
    DEPENDS "${proto}"
)

set(target "key_value_service_client")

# Include generated *.pb.h files
add_library(${target} OBJECT STATIC "${target}.cc" ${proto_srcs} ${grpc_srcs})
set(_PROTOBUF_INCLUDES ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
include_directories(${target} INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
