#COMPILER := g++
COMPILER := clang++ -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions 

FLAGS_WINDOWS := -std=c++17 -pipe -Wa,-mbig-obj  -mwindows -Wl,-E -s -O3 -ffast-math 
FLAGS_WINDOWS_DEBUG := -std=c++17 -pipe -Wa,-mbig-obj -Wl,-E  -s -O3 -ffast-math 
FLAGS_LINUX := -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ # -O3 -ffast-math 

DEFINITIONS_WINDOWS := -DWINDOWS 
DEFINITIONS_LINUX := 

TARGET_ENGINE_WINDOWS := ./build/cetim_engine.exe
TARGET_ENGINE_LINUX := ./build/cetim_engine

SRC_IMGUI :=  ./include/imgui/imgui.cpp ./include/imgui/imgui_widgets.cpp ./include/imgui/imgui_tables.cpp ./include/imgui/imgui_draw.cpp ./include/imgui/backends/imgui_impl_opengl3.cpp ./include/imgui/backends/imgui_impl_glfw.cpp  
SRC_ENGINE := ./src/Cetim_Engine/TMP.cpp  $(SRC_IMGUI)
SRC_READER := ./src/font_reader_gtk/font_reader_gtk.cpp
SRC_COMPILER_LUA = ./src/Lua_Compiler/lua_compiler.cpp

INCLUDE_DIRS := -I./src/Font_Reader -I./src/Cetim_Engine -I./src/Cetim_Engine_Core -I./src/Cetim_Engine_Core/components -I./include -I./include/freetype -I./include/bullet3 -I./include/imgui -I./include/imgui/backends -I./include/nanosvg

LIBS_ENGINE_WINDOWS := -llua -lglfw3  -lglew32 -lopengl32 -lbox2d -lfreetype -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_DEBIAN := -L./libs/linux  -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_ARCH :=  -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
LIBS_ENGINE_POP := -L./libs/linux -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 

PRECOMMAND_WINDOWS := 
PRECOMMAND_LINUX := #flatpak-spawn --host

windows: 
	rm -f $(TARGET_LUA_COMPILER_WINDOWS) $(TARGET_ENGINE_WINDOWS) $(TARGET_FONT_READER_WINDOWS)

	
	$(PRECOMMAND_WINDOWS) $(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS) $(DEFINITIONS_WINDOWS) $(LIBS_ENGINE_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)
	

windows_debug: 
	rm -f $(TARGET_LUA_COMPILER_WINDOWS) $(TARGET_ENGINE_WINDOWS) $(TARGET_FONT_READER_WINDOWS)

	
	$(PRECOMMAND_WINDOWS) $(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_WINDOWS_DEBUG) $(DEFINITIONS_WINDOWS) $(LIBS_ENGINE_WINDOWS) -o $(TARGET_ENGINE_WINDOWS)
 
arch:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGE_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX)

	
	$(PRECOMMAND_LINUX) $(COMPILER) $(SRC_ENGINE) $(INCLUTDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_ARCH) -o $(TARGET_ENGINE_LINUX)

debian:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX)

	
	$(PRECOMMAND_LINUX) $(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_DEBIAN) -o $(TARGET_ENGINE_LINUX)

pop:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_LINUX) 

	
	$(PRECOMMAND_LINUX) $(COMPILER) $(SRC_ENGINE) $(INCLUDE_DIRS) $(FLAGS_LINUX) $(DEFINITIONS_LINUX)  $(LIBS_ENGINE_POP) -o $(TARGET_ENGINE_LINUX)

clean:
	rm -f $(TARGET_LUA_COMPILER_LINUX) $(TARGET_LUA_COMPILER_WINDOWS) $(TARGET_ENGINE_WINDOWS) $(TARGET_ENGINE_LINUX) $(TARGET_FONT_READER_WINDOWS) $(TARGET_FONT_READER_LINUX) ./build/font_reader_gtk.glade ./build/theme.css

