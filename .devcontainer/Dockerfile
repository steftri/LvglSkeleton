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
    pkg-config \
    libsdl2-dev \
    sudo \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Match devcontainer.json remoteUser.
RUN useradd -m -s /bin/bash vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode \
    && chmod 0440 /etc/sudoers.d/vscode

WORKDIR /workspaces

