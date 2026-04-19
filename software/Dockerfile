FROM debian:13-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y ca-certificates

RUN sed -i 's/http:/https:/g' /etc/apt/sources.list.d/*

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv

RUN apt-get update && apt-get install -y \
    git \
    curl \
    udev \
    libusb-1.0-0 \
    build-essential

RUN apt-get update && apt-get install -y \
    libncurses5-dev \
    flex \
    bison

WORKDIR /workspace

ENV PATH="/workspace/venv/bin:$PATH"

ENV PLATFORMIO_CORE_DIR="/workspace/.pio"

CMD ["python3", "--version"]
