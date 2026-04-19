container = platformio-lilygo
port ?= /dev/ttyACM0
manager ?= docker
baud = 115200

  ###########
  # Targets #
  ###########

.PHONY: install
install: docker venv components

.PHONY: docker
docker:
	$(manager) build -t $(container) .

.PHONY: build
build:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) pio run

.PHONY: upload
upload:
	$(manager) run --rm -it --device $(port) --group-add keep-groups -v $(PWD):/workspace $(container) pio run -t upload --upload-port $(port)

.PHONY: menuconfig
menuconfig:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) pio run -t menuconfig

.PHONY: serial
serial:
	minicom -D $(port) -b $(baud)

.PHONY: clean
clean:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) pio run -t clean

.PHONY: purge
purge:
	rm -rvf .pio venv CMakeLists.txt dependencies.lock managed_components components

  ################
  # Dependencies #
  ################

venv:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) python3 -m venv ./venv
	$(manager) run --rm -it -v $(PWD):/workspace $(container) ./venv/bin/pip install --no-cache-dir platformio
	$(manager) run --rm -it -v $(PWD):/workspace $(container) pio pkg install

libs=\
mkdir -p components && \
cd components && \
git clone --depth 1 https://github.com/espressif/arduino-esp32.git arduino && \
cd arduino && \
git submodule update --init --recursive --depth 1 && \
cd .. && \
git clone --recursive --depth 1 --branch V2.5.43 https://github.com/Bodmer/TFT_eSPI.git
components: venv
	$(manager) run --rm -it -v $(PWD):/workspace $(container) bash -c '$(libs)'
