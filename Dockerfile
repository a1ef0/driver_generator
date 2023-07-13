FROM ubuntu:20.04

ARG GPRC_VERSION=1.56.0

ENV DEBIAN_FRONTEND noninteractive

# Install package dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        software-properties-common \
        autoconf \
        automake \
        libtool \
        pkg-config \
        ca-certificates \
        wget \
        git \
        curl \
        vim \
        gdb \
        valgrind \
        cmake
RUN apt-get clean

# snmp
RUN apt-get install -y snmp libsnmp-dev

# gRPC
RUN cd /tmp && \
    apt-get install -y build-essential autoconf libtool pkg-config && \
    git clone --recurse-submodules --depth 1 --shallow-submodules -b v${GPRC_VERSION} https://github.com/grpc/grpc

RUN ls && pwd

RUN export NUM_JOBS=$([ $(uname) = 'Linux' ] && \
        lscpu -p | egrep -v '^#' | wc -l) && \
    cd /tmp/grpc && \
    mkdir -p cmake/build && \
    cd cmake/build && \
    cmake -DgRPC_INSTALL=ON \
        -DProtobuf_install=ON \
        -DgRPC_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=~/.local/ \
        ../.. && \
    make -j${NUM_JOBS} && \
    make install

RUN export PATH="~/.local/bin:$PATH"

# protoc
RUN apt-get install -y protobuf-compiler

WORKDIR /app

COPY grpc/ ./grpc/
COPY protos/ ./protos/
COPY snmp/ ./snmp/
COPY main.cc .
COPY CMakeLists.txt ./

# RUN export CMAKE_PREFIX_PATH=~/.local/include:$CMAKE_PREFIX_PATH

RUN cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local/ -S ./ -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-I/root/.local/include"
RUN cmake --build build --parallel --target driver
