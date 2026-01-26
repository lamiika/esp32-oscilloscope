container=platformio-esp32

.PHONY: docker-build
docker-build:
	docker build -t $(container) .

.PHONY: docker-shell
docker-shell:
	docker run -it -v $(PWD):/workspace $(container) bash

.PHONY: upload
upload:
	docker run -it --device /dev/ttyACM0 -v $(PWD):/workspace $(container) pio run -t upload

.PHONY: depends
depends:
	docker run -it -v $(PWD):/workspace $(container) pio lib install
