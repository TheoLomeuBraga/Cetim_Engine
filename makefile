COMPILER := g++
#COMPILER := clang++ 

#FLAGS_WINDOWS :=  -pipe -Wa,-mbig-obj  -mwindows -Wl,-E -s -O3 -ffast-math 
#FLAGS_WINDOWS_DEBUG :=  -pipe -Wa,-mbig-obj -Wl,-E  -s -O3 -ffast-math 
#FLAGS_LINUX :=  -Wl,-E -static-libgcc -static-libstdc++ # -O3 -ffast-math 

#mingw32-make OPT=2
OPT ?= 0

#the luajit suport causes stacks overflows
JIT ?= 0


SRC_IMGUI = #./include/imgui/imgui.cpp ./include/imgui/backends/imgui_impl_glfw.cpp ./include/imgui/backends/imgui_impl_opengl3.cpp ./include/imgui/imgui_draw.cpp ./include/imgui/imgui_tables.cpp ./include/imgui/imgui_widgets.cpp
INCLUDE_DIRS = -I./include/SDL3 -I./include/recastnavegation -I./src/Font_Reader -I./src/Cetim_Engine -I./src/Cetim_Engine_Core -I./src/Cetim_Engine_Core/components -I./include -I./include/freetype -I./include/bullet3 -I./include/imgui -I./include/imgui/backends -I./include/nanosvg

OS := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	ifeq ($(OPT),0)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -std=c++17 -pipe -Wa,-mbig-obj -Wl,-E  -DDEBUG
	endif
	ifeq ($(OPT),1)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -O2 -DTARGET_FPS=120
	endif
	ifeq ($(OPT),2)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -O3 -ffast-math -DTARGET_FPS=120
	endif
	ifeq ($(OPT),3)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -Ofast -ffast-math -DTARGET_FPS=480 -ftree-parallelize-loops=8
	endif
	TARGET_ENGINE = ./build/cetim_engine.exe
	SRC_ENGINE = ./src/Cetim_Engine/TMP.cpp $(SRC_IMGUI)
	LIBS_ENGINE = -L./libs/x86_64/windows -lz -lboost_serialization -lRecast -lDetour -lDetourCrowd  -lglew32 -lopengl32 -lbox2d -lfreetype -lBulletDynamics -lBulletCollision -lLinearMath -lSDL3 -lSDL3_mixer
	ifeq ($(JIT),0)
		LIBS_ENGINE += -llua -DLUA_NUMBER=float
	endif
	ifeq ($(JIT),1)
		LIBS_ENGINE += -DUSE_LUA_JIT -lluajit
	endif
	
endif
ifeq ($(findstring MINGW,$(OS)),MINGW)
	ifeq ($(OPT),0)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -std=c++17 -pipe -Wa,-mbig-obj -Wl,-E -DDEBUG
	endif
	ifeq ($(OPT),1)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -O2 -DTARGET_FPS=120
	endif
	ifeq ($(OPT),2)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -O3 -ffast-math -DTARGET_FPS=120
	endif
	ifeq ($(OPT),3)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -pipe -Wa,-mbig-obj -mwindows -Wl,-E -s -Ofast -ffast-math -DTARGET_FPS=480 -ftree-parallelize-loops=8
	endif
	TARGET_ENGINE = ./build/cetim_engine.exe
	SRC_ENGINE = ./src/Cetim_Engine/TMP.cpp $(SRC_IMGUI)
	LIBS_ENGINE = -L./libs/x86_64/windows -lz -lboost_serialization -lRecast -lDetour -lDetourCrowd  -lglew32 -lopengl32 -lbox2d -lfreetype -lBulletDynamics -lBulletCollision -lLinearMath -lSDL3 -lSDL3_mixer
	ifeq ($(JIT),0)
		LIBS_ENGINE += -llua -DLUA_NUMBER=float
	endif
	ifeq ($(JIT),1)
		LIBS_ENGINE += -DUSE_LUA_JIT -lluajit
	endif
endif
ifeq ($(OS),Linux)
	ifeq ($(OPT),0)
		COMPILER_FLAGS = -pedantic -DLLVM_ENABLE_LTO=THIN -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -g -fsanitize=address -DDEBUG
	endif
		ifeq ($(OPT),1)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -O2 -DTARGET_FPS=120
	endif
	ifeq ($(OPT),2)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -O3 -ffast-math -flto -DTARGET_FPS=120
	endif
	ifeq ($(OPT),3)
		COMPILER_FLAGS = -DLLVM_ENABLE_LTO=THIN -funroll-loops -finline-functions -std=c++17 -Wl,-E -static-libgcc -static-libstdc++ -Ofast -ffast-math -flto -DTARGET_FPS=480 -ftree-parallelize-loops=8
	endif
	TARGET_ENGINE = ./build/cetim_engine
	SRC_ENGINE = ./src/Cetim_Engine/TMP.cpp $(SRC_IMGUI)
	LIBS_ENGINE = -L./libs/x86_64/linux -lz -lboost_serialization -lRecast -lDetour -lDetourCrowd  -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-network -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath  -lSDL3 -lSDL3_mixer
	ifeq ($(JIT),0)
		LIBS_ENGINE += -llua -DLUA_NUMBER=float
	endif
	ifeq ($(JIT),1)
		LIBS_ENGINE += -lluajit -DUSE_LUA_JIT
	endif
		
endif

all: 
	
	rm -f $(TARGET_ENGINE)
	$(COMPILER) $(COMPILER_FLAGS) $(SRC_ENGINE) $(INCLUDE_DIRS) $(LIBS_ENGINE) -o $(TARGET_ENGINE)

clean:
	rm -f $(TARGET_ENGINE)

