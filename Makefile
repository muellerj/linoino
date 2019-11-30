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
	@find tmp/a -type f -name "*.m4a" | \
		while read f; do \
			newname=$$(bin/bestname "$$(dirname "$$f")/$$(basename "$$f" ".m4a").mp3"); \
			echo "Converting $$f -> $$newname"; \
			ffmpeg -i "$$f" -acodec libmp3lame -aq 2 "$$newname"; \
			rm -f "$$f"; \
		done

copy:
	rsync -zarv --progress --include="*/" --include="*.mp3" --exclude="*" $(TC)/ $(SDCARD)/
	rm -rf $(SDCARD)/**/.*

clean:
	rm -rf tmp/*.aiff
	rm -rf tmp/*.wav
	rm -rf sd-card
	rm -rf out

audio_messages:
	bin/create_audio_messages $(TC) msg/audio_messages.txt

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
