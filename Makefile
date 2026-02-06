container = platformio-lilygo
port ?= /dev/ttyACM0
manager ?= docker
baud = 115200

.PHONY: docker-build
docker-build:
	$(manager) build -t $(container) .

.PHONY: docker-shell
docker-shell:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) bash

.PHONY: build
build:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) pio run

.PHONY: upload
upload:
	$(manager) run --rm -it --device $(port) --group-add keep-groups -v $(PWD):/workspace $(container) pio run -t upload --upload-port $(port)

.PHONY: depends
depends:
	$(manager) run --rm -it -v $(PWD):/workspace $(container) pio lib install

.PHONY: serial
serial:
	minicom -D $(port) -b $(baud)
