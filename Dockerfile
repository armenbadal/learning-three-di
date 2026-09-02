# Development and build container for learning-three-di (C++23 renderer).
# Toolchain: GCC 16, CMake 4.2, Ninja, Catch2 3, GLFW and OpenGL.
#
# Build:
#   docker build -t learning-three-di:dev .
# Run the already-built project:
#   docker run --rm learning-three-di:dev ctest --test-dir /workspace/build --output-on-failure
# Develop (mount your source tree and build it):
#   docker run --rm -it -v ${PWD}:/workspace learning-three-di:dev

ARG UBUNTU_VERSION=24.04
FROM ubuntu:${UBUNTU_VERSION}

ENV DEBIAN_FRONTEND=noninteractive \
    CMAKE_VERSION=4.2.0 \
    GCC_VERSION=16

# Base tools + build essentials + dev utilities
RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        wget \
        gnupg \
        software-properties-common \
        build-essential \
        ninja-build \
        ccache \
        pkg-config \
        git \
        zip \
        unzip \
        gdb \
        less \
        vim \
        nano \
        jq \
        python3-jinja2 \
        catch2 \
        libglfw3-dev \
        autoconf \
        autoconf-archive \
        automake \
        libtool \
        libgl1-mesa-dev \
        libx11-dev \
        libxrandr-dev \
        libxinerama-dev \
        libxcursor-dev \
        libxi-dev \
        libxext-dev \
        xorg-dev \
    && git config --system http.version HTTP/1.1 \
    && rm -rf /var/lib/apt/lists/*

# GCC ${GCC_VERSION} from ubuntu-toolchain-r/test PPA (noble ships only up to 14)
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        gcc-${GCC_VERSION} \
        g++-${GCC_VERSION} \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 100 \
    && update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-${GCC_VERSION} 100 \
    && update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-${GCC_VERSION} 100 \
    && rm -rf /var/lib/apt/lists/*

# CMake ${CMAKE_VERSION} prebuilt binary.  uname uses the names published by
# Kitware (x86_64 and aarch64), unlike Docker's amd64 build-platform value.
RUN cmake_arch="$(uname -m)" \
    && wget -q "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${cmake_arch}.tar.gz" \
        -O /tmp/cmake.tar.gz \
    && mkdir -p /opt/cmake \
    && tar -xzf /tmp/cmake.tar.gz --strip-components=1 -C /opt/cmake \
    && ln -sf /opt/cmake/bin/cmake /usr/local/bin/cmake \
    && ln -sf /opt/cmake/bin/ctest /usr/local/bin/ctest \
    && ln -sf /opt/cmake/bin/cpack /usr/local/bin/cpack \
    && rm /tmp/cmake.tar.gz

WORKDIR /workspace

# Validate that the container can build every library, test, demo and the
# showcase.  The source remains available in the resulting image at /workspace.
COPY . /workspace
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
    && cmake --build build --parallel \
    && ctest --test-dir build --output-on-failure

CMD ["/bin/bash"]
