FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    ninja-build \
    gdb \
    python3 \
    python3-pip \
    python3-venv \
    pkg-config \
    libgtest-dev \
    libsdl2-dev \
    sudo \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO and the Python-side codegen/runtime deps it may try to
# fetch on first test run, then expose the CLI globally.
RUN python3 -m venv /opt/platformio \
    && /opt/platformio/bin/pip install --no-cache-dir --upgrade pip setuptools wheel \
    && /opt/platformio/bin/pip install --no-cache-dir platformio protobuf grpcio-tools \
    && ln -sf /opt/platformio/bin/pio /usr/local/bin/pio \
    && ln -sf /opt/platformio/bin/platformio /usr/local/bin/platformio

WORKDIR /workspaces

