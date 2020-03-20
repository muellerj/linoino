all: upload

PORT=/dev/cu.wchusbserialfd3140
OUT=.pio/build/nanoatmega328/firmware.hex
SRC=src/*
TC=/Volumes/tiggercloud/Linuino
SDCARD=/Volumes/LINUINO

setup:
	bin/setup

dummysongs:
	bin/create_dummy_songs $(TC)

convert:
	bin/convert_songs $(TC)

stats:
	bin/showstats

copy:
	rsync -zarv --delete --progress --include="*/" --include="*.mp3" --exclude="*" $(TC)/ $(SDCARD)/
	find $(SDCARD) -name '.*' | xargs rm -rf

clean:
	rm -rf tmp/*.aiff
	rm -rf tmp/*.wav
	rm -rf sd-card
	rm -rf .pio/build

audio_messages:
	bin/create_audio_messages $(TC) msg/audio_messages.txt

deploy: upload monitor

compile: $(OUT)

monitor:
	@platformio device monitor

upload: $(OUT)
	@platformio run -t upload

$(OUT): $(SRC)
	@platformio run
