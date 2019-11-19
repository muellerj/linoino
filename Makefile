all: upload

BOARD=arduino:avr:nano:cpu=atmega328old
PORT=/dev/ttyACM0
OUT=out/linuino
SRC=linuino.ino

setup:
	sudo bin/setup

clean:
	rm -rf tmp/*.aiff
	rm -rf tmp/*.wav
	rm -rf sd-card

soundfiles:
	bin/create_soundfiles msg/audio_messages_de.txt

compile: $(OUT)

upload: $(OUT)
	arduino-cli upload \
		--fqbn $(BOARD) \
		--port $(PORT) \
		--input $< \
		--verify \
		.

$(OUT): $(SRC)
	mkdir -p out
	arduino-cli compile \
		--fqbn $(BOARD) \
		--output $(OUT) \
		.
