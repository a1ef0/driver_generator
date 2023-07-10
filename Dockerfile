FROM ubuntu:20.04

ARG GPRC_VERSION=1.56.0
ARG NUM_JOBS=12

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

RUN cd /tmp/grpc && \
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

WORKDIR /app

COPY grpc/ ./grpc/
COPY protos/ ./protos/
COPY snmp/ ./snmp/
COPY CMakeLists.txt .

RUN cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local/
RUN cmake --build build --config Release --parallel
