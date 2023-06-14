COMPILER := g++

FLAGS_WINDOWS := -std=c++17 -pipe -Wa,-mbig-obj  -mwindows
FLAGS_WINDOWS_DEBUG := -std=c++17 -pipe -Wa,-mbig-obj 
FLAGS_LINUX := -std=c++17

DEFINITIONS_WINDOWS := -DWINDOWS -D_HAS_STD_BYTE=0 
DEFINITIONS_LINUX := -DLINUX

TARGET_ENGINE_WINDOWS := ./build/engine_theo_master_peace.exe
TARGET_ENGINE_LINUX := ./build/engine_theo_master_peace

TARGET_FONT_READER_WINDOWS := ./build/font_reader_gtk.exe
TARGET_FONT_READER_LINUX := ./build/font_reader_gtk

SRC_ENGINE := ./src/Theo_Master_Peace/TMP.cpp
SRC_READER := ./src/font_reader_gtk/font_reader_gtk.cpp

INCLUDE_DIRS := -I./src/Font_Reader -I./src/Theo_Master_Peace -I./src/Theo_Master_Peace_Core -I./src/Theo_Master_Peace_Core/components -I./include -I./include/freetype -I./include/bullet3
HEADERS := $(wildcard $(addsuffix /*.h,$(INCLUDE_DIRS)))

LIBS_ENGINE_WINDOWS := -llua -lglfw3  -lglew32 -lopengl32 -lbox2d  -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_DEBIAN := -llua5.4 -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_ARCH := -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 

LIBS_FONT_READER_WINDOWS := -lfreetype `pkg-config --cflags --libs gtk+-3.0`
LIBS_FONT_READER_LINUX := -lfreetype `pkg-config --cflags --libs gtk+-3.0`

windows: 
	rm -f $(TARGET_ENGINE_WINDOWS) $(TARGET_FONT_READER_WINDOWS) 
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade

	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_ENGINE_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)
	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_WINDOWS)

windows_debug: 
	rm -f $(TARGET_ENGINE_WINDOWS) $(TARGET_FONT_READER_WINDOWS) 
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade

	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS_DEBUG) $(DEFINITIONS_WINDOWS) $(LIBS_ENGINE_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)
	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_WINDOWS_DEBUG) $(DEFINITIONS_WINDOWS) $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_WINDOWS)

arch:
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	rm -f $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX)

	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_ARCH) -o $(TARGET_ENGINE_LINUX)
	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_LINUX)

debian:
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	rm -f $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX)

	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_DEBIAN) -o $(TARGET_ENGINE_LINUX)
	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_LINUX)

clean:
	rm -f $(TARGET_ENGINE_WINDOWS) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_WINDOWS) $(TARGET_FONT_READER_LINUX) ./build/font_reader_gtk.glade

