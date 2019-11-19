all: upload

BOARD=arduino:avr:nano:cpu=atmega328old
PORT=/dev/ttyACM0
OUT=out/linuino
SRC=linuino.ino

setup:
	bin/setup

soundfiles:
	bin/create_soundfiles

upload: $(OUT)
	arduino-cli upload \
		--fqbn $(BOARD) \
		--port $(PORT) \
		--input $< \
		--verify \
		.

compile: $(SRC)
	mkdir -p out
	arduino-cli compile \
		--fqbn $(BOARD) \
		--output $(OUT) \
		.
