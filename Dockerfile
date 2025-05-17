FROM ubuntu:22.04

# --- базовая настройка окружения ---
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gdb \
    python3 \
    python3-dev \
    python3-pip \
    python3-venv \
    python-is-python3 \
    git \
    nano \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Проверка Python include path (для CLion)
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# Установим wheel чтобы избежать ошибок при pip install
RUN pip install --upgrade pip setuptools wheel

# Рабочая директория
WORKDIR /workspace
