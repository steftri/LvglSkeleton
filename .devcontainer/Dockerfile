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
    libsdl2-dev \
    sudo \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO in an isolated venv and expose the CLI globally.
RUN python3 -m venv /opt/platformio/penv \
    && /opt/platformio/penv/bin/pip install --no-cache-dir --upgrade pip setuptools wheel \
    && /opt/platformio/penv/bin/pip install --no-cache-dir platformio \
    && ln -sf /opt/platformio/penv/bin/pio /usr/local/bin/pio

# Match devcontainer.json remoteUser.
RUN useradd -m -s /bin/bash vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode \
    && chmod 0440 /etc/sudoers.d/vscode

WORKDIR /workspaces

