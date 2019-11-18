all: upload

BOARD=arduino:avr:nano:cpu=atmega328old
PORT=/dev/ttyACM0
PROJECT=linuino

setup:
	bin/setup

upload: compile
	arduino-cli upload \
		--fqbn $(BOARD) \
		--port $(PORT) \
		--input out/$(PROJECT) \
		--verify \
		.

compile:
	mkdir -p out
	arduino-cli compile \
		--fqbn $(BOARD) \
		--output out/$(PROJECT) \
		.
