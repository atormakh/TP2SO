include Makefile.inc

SAMPLE_DATA=0001-userlandDataModule.bin

all: userlandCodeModule userlandDataModule

userlandCodeModule:
	cd UserlandCodeModule; make

userlandDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd UserlandCodeModule; make clean
	rm -rf *.bin


.PHONY: userlandCodeModule all clean
