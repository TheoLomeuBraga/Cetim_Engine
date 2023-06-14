COMPILER := g++

FLAGS_WINDOWS := -std=c++17 -pipe -Wa,-mbig-obj
FLAGS_LINUX := -std=c++17

DEFINITIONS_WINDOWS := -DWINDOWS -D_HAS_STD_BYTE=0 
DEFINITIONS_LINUX := -DLINUX

TARGET_ENGINE_WINDOWS := ./build/engine_theo_master_peace.exe
TARGET_ENGINE_LINUX := ./build/engine_theo_master_peace

TARGET_READER_WINDOWS := ./build/font_reader_gtk.exe
TARGET_READER_LINUX := ./build/font_reader_gtk

SRC_ENGINE := ./src/Theo_Master_Peace/TMP.cpp
SRC_READER := ./src/font_reader_gtk/font_reader_gtk.cpp

INCLUDE_DIRS := -I./src/Font_Reader -I./src/Theo_Master_Peace -I./src/Theo_Master_Peace_Core -I./src/Theo_Master_Peace_Core/components -I./include -I./include/freetype -I./include/bullet3
HEADERS := $(wildcard $(addsuffix /*.h,$(INCLUDE_DIRS)))

LIBS_WINDOWS := -llua -lglfw3  -lglew32 -lopengl32 -lbox2d -lfreetype -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath
LIBS_LINUX := -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 



headers:
	$(COMPILER) $(FLAGS_LINUX) $(DEFINITIONS_LINUX) $(INCLUDE_DIRS) -o $(HEADERS)


print_headers:
	@echo $(HEADERS)