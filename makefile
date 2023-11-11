COMPILER := g++

FLAGS_WINDOWS := -std=c++17 -pipe -Wa,-mbig-obj  -mwindows -Wl,-E -O3
FLAGS_WINDOWS_DEBUG := -std=c++17 -pipe -Wa,-mbig-obj  -Wl,-E
FLAGS_LINUX := -std=c++17 -Wl,-E -static-libgcc -O3 #-pg -g

DEFINITIONS_WINDOWS := -DWINDOWS -D_HAS_STD_BYTE=0 
DEFINITIONS_LINUX := -DLINUX -DUNIX 

TARGET_ENGINE_WINDOWS := ./build/engine_theo_master_pice.exe
TARGET_ENGINE_LINUX := ./build/engine_theo_master_pice

TARGET_FONT_READER_WINDOWS := ./build/font_reader_gtk.exe
TARGET_FONT_READER_LINUX := ./build/font_reader_gtk

TARGET_LUA_COMPILER_WINDOWS := ./build/lua_conpiler.exe
TARGET_LUA_COMPILER_LINUX := ./build/lua_conpiler

SRC_IMGUI :=  ./include/imgui/imgui.cpp ./include/imgui/imgui_widgets.cpp ./include/imgui/imgui_tables.cpp ./include/imgui/imgui_draw.cpp ./include/imgui/backends/imgui_impl_opengl3.cpp ./include/imgui/backends/imgui_impl_glfw.cpp  
SRC_ENGINE := ./src/Theo_Master_Pice/TMP.cpp  $(SRC_IMGUI)
SRC_READER := ./src/font_reader_gtk/font_reader_gtk.cpp
SRC_COMPILER_LUA = ./src/Lua_Compiler/lua_compiler.cpp

INCLUDE_DIRS := -I./src/Font_Reader -I./src/Theo_Master_Pice -I./src/Theo_Master_Pice_Core -I./src/Theo_Master_Pice_Core/components -I./include -I./include/freetype -I./include/bullet3 -I./include/imgui -I./include/imgui/backends -I./include/nanosvg

LIBS_ENGINE_WINDOWS := -llua -lglfw3  -lglew32 -lopengl32 -lbox2d  -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_DEBIAN := -L./libs/linux  -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_ARCH :=  -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_POP := -L./libs/linux  -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 

LIBS_FONT_READER_WINDOWS := -lfreetype `pkg-config --cflags --libs gtk+-3.0`
LIBS_FONT_READER_LINUX := -lfreetype `pkg-config --cflags --libs gtk+-3.0`

LIBS_LUA_COMPILER_WINDOWS =   -llua
LIBS_LUA_COMPILER_LINUX =  -L./libs/linux  -llua

windows: 
	rm -f $(TARGET_LUA_COMPILER_WINDOWS) $(TARGET_ENGINE_WINDOWS) $(TARGET_FONT_READER_WINDOWS) ./build/font_reader_gtk.glade ./build/theme.css
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	cp ./src/font_reader_gtk/theme.css ./build/theme.css

	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_WINDOWS)
	$(COMPILER) $(SRC_COMPILER_LUA) $(FLAGS_WINDOWS) $(LIBS_LUA_COMPILER_WINDOWS) $(INCLUDE_DIRS) -o $(TARGET_LUA_COMPILER_WINDOWS)
	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_ENGINE_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)
	

windows_debug: 
	rm -f $(TARGET_LUA_COMPILER_WINDOWS) $(TARGET_ENGINE_WINDOWS) $(TARGET_FONT_READER_WINDOWS)  ./build/font_reader_gtk.glade ./build/theme.css
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	cp ./src/font_reader_gtk/theme.css ./build/theme.css

	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_WINDOWS_DEBUG) $(DEFINITIONS_WINDOWS) $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_WINDOWS)
	$(COMPILER) $(SRC_COMPILER_LUA) $(FLAGS_WINDOWS_DEBUG) $(LIBS_LUA_COMPILER_WINDOWS) $(INCLUDE_DIRS) -o $(TARGET_LUA_COMPILER_WINDOWS)
	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS_DEBUG) $(DEFINITIONS_WINDOWS) $(LIBS_ENGINE_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)

arch:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGE_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX) ./build/font_reader_gtk.glade ./build/theme.css
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	cp ./src/font_reader_gtk/theme.css ./build/theme.css

	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_LINUX)
	$(COMPILER) $(SRC_COMPILER_LUA) $(FLAGS_LINUX) $(LIBS_LUA_COMPILER_LINUX) $(INCLUDE_DIRS) -o $(TARGET_LUA_COMPILER_LINUX)
	$(COMPILER) $(SRC_ENGINE) $(INCLUTDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_ARCH) -o $(TARGET_ENGINE_LINUX)

debian:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX) ./build/font_reader_gtk.glade ./build/theme.css
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	cp ./src/font_reader_gtk/theme.css ./build/theme.css

	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_LINUX)
	$(COMPILER) $(SRC_COMPILER_LUA) $(FLAGS_LINUX) $(LIBS_LUA_COMPILER_LINUX) $(INCLUDE_DIRS) -o $(TARGET_LUA_COMPILER_LINUX)
	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_DEBIAN) -o $(TARGET_ENGINE_LINUX)

pop:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX) ./build/font_reader_gtk.glade ./build/theme.css
	cp ./src/font_reader_gtk/font_reader_gtk.glade ./build/font_reader_gtk.glade
	cp ./src/font_reader_gtk/theme.css ./build/theme.css

	$(COMPILER) $(SRC_READER) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_FONT_READER_LINUX) -o $(TARGET_FONT_READER_LINUX)
	$(COMPILER) $(SRC_COMPILER_LUA) $(FLAGS_LINUX) $(LIBS_LUA_COMPILER_LINUX) $(INCLUDE_DIRS) -o $(TARGET_LUA_COMPILER_LINUX)
	$(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_POP) -o $(TARGET_ENGINE_LINUX)

clean:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGET_LUA_COMPILER_WINDOWS) $(TARGET_ENGINE_WINDOWS) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_WINDOWS) $(TARGET_FONT_READER_LINUX) ./build/font_reader_gtk.glade ./build/theme.css

