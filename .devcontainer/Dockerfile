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

# Match devcontainer.json remoteUser.
RUN useradd -m -s /bin/bash vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode \
    && chmod 0440 /etc/sudoers.d/vscode

# Install PlatformIO and the Python-side codegen/runtime deps it may try to
# fetch on first test run, then expose the CLI globally.
RUN install -d -o vscode -g vscode /opt/platformio \
    && runuser -u vscode -- python3 -m venv /opt/platformio/penv \
    && runuser -u vscode -- /opt/platformio/penv/bin/pip install --no-cache-dir --upgrade pip setuptools wheel \
    && runuser -u vscode -- /opt/platformio/penv/bin/pip install --no-cache-dir platformio protobuf grpcio-tools \
    && ln -sf /opt/platformio/penv/bin/pio /usr/local/bin/pio \
    && ln -sf /opt/platformio/penv/bin/platformio /usr/local/bin/platformio

WORKDIR /workspaces

