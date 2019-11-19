all: upload

BOARD=arduino:avr:nano:cpu=atmega328old
PORT=/dev/ttyACM0
OUT=out/linuino
SRC=linuino.ino

setup:
	bin/setup

dummycard:
	bin/create_dummy_songs

clean:
	rm -rf tmp/*.aiff
	rm -rf tmp/*.wav
	rm -rf sd-card
	rm -rf out

audio_messages:
	bin/create_audio_messages msg/audio_messages.txt

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
