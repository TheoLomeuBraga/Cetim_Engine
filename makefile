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

LIBS_WINDOWS := -llua -lglfw3  -lglew32 -lopengl32 -lbox2d -lfreetype -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath `pkg-config --cflags --libs gtk+-3.0`
LIBS_LINUX := -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath `pkg-config --cflags --libs gtk+-3.0`




	
OBJ := $(SRC:.cpp=.o)

windows: 
	rm -f $(OBJ) $(TARGET_ENGINE_WINDOWS) $(TARGET_READER_WINDOWS) 
	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)
	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)


linux:
	rm -f $(OBJ) $(TARGET_ENGINE_LINUX) $(TARGET_READER_LINUX)
	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_LINUX) -o $(TARGET_ENGINE_LINUX)
	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_LINUX) -o $(TARGET_ENGINE_LINUX)





clean:
	rm -f $(OBJ) $(TARGET_ENGINE_WINDOWS) $(TARGET_ENGINE_LINUX) $(TARGET_READER_WINDOWS) $(TARGET_READER_LINUX)


print_headers:
	@echo $(HEADERS)