FROM debian:13-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    git \
    curl \
    ca-certificates \
    udev \
    libusb-1.0-0 \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /

RUN python3 -m venv /venv

RUN /venv/bin/pip install --no-cache-dir platformio

ENV PATH="/venv/bin:$PATH"

WORKDIR /tmp

COPY platformio.ini .

RUN pio pkg install

WORKDIR /workspace

CMD ["platformio", "--version"]
