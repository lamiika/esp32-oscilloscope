container = platformio-esp32
 
manager ?= docker 
port ?= /dev/ttyACM0

.PHONY: docker-build
docker-build:
	${manager} build -t $(container) .

.PHONY: docker-shell
docker-shell:
	${manager} run -it -v $(PWD):/workspace $(container) bash

.PHONY: upload
upload:
	${manager} run -it --device ${port} --group-add keep-groups -v $(PWD):/workspace $(container) pio run -t upload --upload-port ${port}
	
.PHONY: depends
depends:
	${manager} run -it -v $(PWD):/workspace $(container) pio lib install
