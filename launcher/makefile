
NAME := play
EPATH := /build/cetim_engine
CPATH := /projects/HMTF
LPATHL := /libs/x86_64/linux
LPATHW := /libs/x86_64/windows/dll



compile_cmd = gcc
OS := $(shell uname -s)
ifeq ($(OS),Windows_NT)	

endif
ifeq ($(findstring MINGW,$(OS)),MINGW)

endif
ifeq ($(OS),Linux)
	compile_cmd += launch.c -o ../$(NAME) -DEPATH='"$(EPATH)"' -DCPATH='"$(CPATH)"' -DLPATHL='"$(LPATHL)"' -DLPATHW='"$(LPATHW)"'
endif

all:
	cd ..
	
	
	$(compile_cmd)

clean:
	rm -f ../$(NAME).exe ../$(NAME)