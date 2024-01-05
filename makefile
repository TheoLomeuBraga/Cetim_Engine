#COMPILER := g++
COMPILER := clang++ 

#FLAGS_WINDOWS :=  -pipe -Wa,-mbig-obj  -mwindows -Wl,-E -s -O3 -ffast-math 
#FLAGS_WINDOWS_DEBUG :=  -pipe -Wa,-mbig-obj -Wl,-E  -s -O3 -ffast-math 
#FLAGS_LINUX :=  -Wl,-E -static-libgcc -static-libstdc++ # -O3 -ffast-math 

OPTIMIZATION ?= 0

SRC_IMGUI :=  ./include/imgui/imgui.cpp ./include/imgui/imgui_widgets.cpp ./include/imgui/imgui_tables.cpp ./include/imgui/imgui_draw.cpp ./include/imgui/backends/imgui_impl_opengl3.cpp ./include/imgui/backends/imgui_impl_glfw.cpp  

OS := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	ifeq ($(OPTIMIZATION),0)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -Wl,-E -s -g
	endif
	ifeq ($(OPTIMIZATION),1)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -O2
	endif
	ifeq ($(OPTIMIZATION),2)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -O3 -ffast-math 
	endif
	TARGET_ENGINE = ./build/cetim_engine.exe
	SRC_ENGINE = ./src/Cetim_Engine/TMP.cpp $(SRC_IMGUI)
	INCLUDE_DIRS = -I./src/Font_Reader -I./src/Cetim_Engine -I./src/Cetim_Engine_Core -I./src/Cetim_Engine_Core/components -I./include -I./include/freetype -I./include/bullet3 -I./include/imgui -I./include/imgui/backends -I./include/nanosvg
	LIBS_ENGINE = -llua -lglfw3 -lglew32 -lopengl32 -lbox2d -lfreetype -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
endif
ifeq ($(OS),Linux)
	ifeq ($(OPTIMIZATION),0)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -g
	endif
		ifeq ($(OPTIMIZATION),1)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -O2
	endif
	ifeq ($(OPTIMIZATION),2)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -O3 -ffast-math 
	endif
		TARGET_ENGINE = ./build/cetim_engine
		SRC_ENGINE = ./src/Cetim_Engine/TMP.cpp $(SRC_IMGUI)
		INCLUDE_DIRS = -I./src/Font_Reader -I./src/Cetim_Engine -I./src/Cetim_Engine_Core -I./src/Cetim_Engine_Core/components -I./include -I./include/freetype -I./include/bullet3 -I./include/imgui -I./include/imgui/backends -I./include/nanosvg
		LIBS_ENGINE = -L./libs/linux -llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 
endif

all: 
	
	rm -f $(TARGET_ENGINE)
	$(COMPILER) $(COMPILER_FLAGS) $(SRC_ENGINE) $(INCLUDE_DIRS) $(LIBS_ENGINE) -o $(TARGET_ENGINE)

clean:
	rm -f $(TARGET_ENGINE)

