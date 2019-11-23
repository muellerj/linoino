all: upload

BOARD=arduino:avr:nano:cpu=atmega328old
PORT=/dev/cu.wchusbserialfd3140
OUT=out/linuino
SRC=linuino.ino

RED=\033[0;31m
GREEN=\033[0;32m
RESET=\033[0m 

define ok
	([ $$? -eq 0 ] && printf "\n$(GREEN)$(1) OK$(RESET)\n") || \
	printf "\n$(RED)$(1) FAIL$(RESET)\n"
endef

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

monitor:
	# Note that C-A C-\ exits screen
	screen $(PORT) 115200

upload: $(OUT)
	@arduino-cli upload \
		--fqbn $(BOARD) \
		--port $(PORT) \
		--input $< \
		--verify \
		.  && $(call ok, "UPLOAD")

$(OUT): $(SRC)
	@mkdir -p out
	@arduino-cli compile \
		--fqbn $(BOARD) \
		--output $(OUT) \
		.  && $(call ok, "COMPILER")
