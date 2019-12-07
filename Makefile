all: upload

BOARD=arduino:avr:nano:cpu=atmega328old
PORT=/dev/cu.wchusbserialfd3140
OUT=out/linuino
SRC=linuino.ino lib/*.cpp lib/*.h
TC=/Volumes/tiggercloud/Linuino
SDCARD=/Volumes/LINUINO

RED=\033[0;31m
GREEN=\033[0;32m
RESET=\033[0m 

define ok
	([ $$? -eq 0 ] && printf "\n$(GREEN)$(1) OK$(RESET)\n") || \
	printf "\n$(RED)$(1) FAIL$(RESET)\n"
endef

setup:
	bin/setup

dummysongs:
	bin/create_dummy_songs $(TC)

convert:
	bin/convert_songs $(TC)

copy:
	rsync -zarv --progress --include="*/" --include="*.mp3" --exclude="*" $(TC)/ $(SDCARD)/
	find $(SDCARD) -name '.*' | xargs rm -rf

clean:
	rm -rf tmp/*.aiff
	rm -rf tmp/*.wav
	rm -rf sd-card
	rm -rf out

audio_messages:
	bin/create_audio_messages $(TC) msg/audio_messages.txt

deploy: upload monitor

compile: $(OUT).hex

$(OUT).hex: $(OUT).elf

monitor:
	# Note that C-A C-\ exits screen
	screen $(PORT) 115200

upload: $(OUT).hex
	@arduino-cli upload \
		--fqbn $(BOARD) \
		--port $(PORT) \
		--input $< \
		--verify \
		.  && $(call ok, "UPLOAD")

$(OUT).elf: $(SRC)
	@mkdir -p out
	@arduino-cli compile \
		--fqbn $(BOARD) \
		--output $(OUT) \
		.  && $(call ok, "COMPILER")
