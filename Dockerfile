# Dev container for learning-three-di (C++23 software rasterizer)
# Toolchain: GCC 16, CMake 4.2, Ninja, Catch2 3, ccache
#
# Build:
#   docker build -t learning-three-di:dev .
# Run (mount your source tree):
#   docker run --rm -it -v ${PWD}:/workspace learning-three-di:dev

ARG UBUNTU_VERSION=24.04
FROM ubuntu:${UBUNTU_VERSION}

ENV DEBIAN_FRONTEND=noninteractive \
    CMAKE_VERSION=4.2.0 \
    GCC_VERSION=16 \
    TARGETARCH=${TARGETARCH:-x86_64}

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
        unzip \
        gdb \
        less \
        vim \
        nano \
        jq \
        catch2 \
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

# CMake ${CMAKE_VERSION} prebuilt binary (do NOT use the source tarball -- it has no bin/)
RUN wget -q "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${TARGETARCH}.tar.gz" \
        -O /tmp/cmake.tar.gz \
    && mkdir -p /opt/cmake \
    && tar -xzf /tmp/cmake.tar.gz --strip-components=1 -C /opt/cmake \
    && ln -sf /opt/cmake/bin/cmake /usr/local/bin/cmake \
    && ln -sf /opt/cmake/bin/ctest /usr/local/bin/ctest \
    && ln -sf /opt/cmake/bin/cpack /usr/local/bin/cpack \
    && rm /tmp/cmake.tar.gz

# Sanity check
RUN gcc --version && cmake --version && ninja --version && pkg-config --modversion catch2

WORKDIR /workspace

CMD ["/bin/bash"]