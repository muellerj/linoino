all: upload

PORT=/dev/cu.wchusbserialfd3140
OUT=.pio/build/nanoatmega328/firmware.hex
SRC=src/*

setup:
	bin/setup

checkenv:
	bin/checkenv

dummysongs: checkenv
	bin/create_dummy_songs $(CARDSRC)

convert: checkenv
	bin/convert_songs $(CARDSRC)/*

stats:
	bin/showstats

copy: checkenv
	rsync -zarv --delete --progress --include="*/" --include="*.mp3" --exclude="*" $(CARDSRC)/ $(CARDDST)/
	find $(CARDDST) -name '.*' | xargs rm -rf

clean:
	rm -rf tmp/*.aiff
	rm -rf tmp/*.wav
	rm -rf .pio/build

audio_messages: checkenv
	bin/create_audio_messages $(CARDSRC) msg/audio_messages.txt

deploy: upload monitor

compile: $(OUT)

monitor:
	@platformio device monitor

upload: $(OUT)
	@platformio run -t upload

$(OUT): $(SRC)
	@platformio run

@PHONY: checkenv
