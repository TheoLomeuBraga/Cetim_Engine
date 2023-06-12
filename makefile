compiler_windows=g++
compiler_linux=g++

engine_cpp_path=./src/Theo_Master_Peace/TMP.cpp

font_reader_cpp_path=./src/Font_Reader_2/font_reader_2.cpp

font_reader_cpp_ui_path=./src/font_reader_gtk/font_reader_gtk.cpp

tags_windows=-std=c++17 -pipe -Wa,-mbig-obj

includes=-I./src/Font_Reader -I./src/Theo_Master_Peace -I./src/Theo_Master_Peace_Core -I./include -I./include/freetype -I./include/bullet3 

build_path=./build



libs_path_windows=-L./libs/windows

definitions_windows=-DWINDOWS -D_HAS_STD_BYTE=0 

libs_windows=-llua -lglfw3  -lglew32 -lopengl32 -lbox2d -lfreetype -lsfml-audio-s -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath


windows:
	
	${compiler_windows} ${tags_windows} ${engine_cpp_path}  ${includes} ${definitions_windows} ${libs_path_windows} ${libs_windows} -o ${build_path}/engine_theo_master_peace.exe 

	#rm ${build_path}/font_reader_gtk.glade
	#cp ./src/font_reader_gtk/font_reader_gtk.glade ${build_path}/font_reader_gtk.glade
	${compiler_linux}  ${tags_windows} ${font_reader_cpp_ui_path} `pkg-config --cflags --libs gtk+-3.0`  ${includes} ${definitions_windows} ${libs_path_windows} ${libs_windows} -o ${build_path}/font_reader_gtk.exe


tags_linux=

libs_path_linux=-L./libs/linux

libs_linux=-llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath 

definitions_linux=-DLINUX

linux:
	${compiler_linux} ${tags_linux} ${engine_cpp_path}  ${includes} ${definitions_linux} ${libs_path_linux} ${libs_linux} -o ${build_path}/engine_theo_master_peace 
	#${compiler_linux} ${tags_linux} ${font_reader_cpp_path}  ${includes} ${definitions_linux} ${libs_path_linux} ${libs_linux} -o ${build_path}/font_reader_2

	cp ./src/font_reader_gtk/font_reader_gtk.glade ${build_path}/font_reader_gtk.glade
	${compiler_linux} ${tags_linux} ${font_reader_cpp_ui_path} `pkg-config --cflags --libs gtk+-3.0`  ${includes} ${definitions_linux} ${libs_path_linux} ${libs_linux} -o ${build_path}/font_reader_gtk

linux_debug:
	${compiler_linux} -g ${tags_linux} ${engine_cpp_linux_path}  ${includes} ${definitions_linux} ${libs_path_linux} ${libs_linux} -o ${build_path}/engine_theo_master_peace 

linux_test:
	${compiler_linux} ${tags_linux} ./src/tests/bullet.cpp  ${includes} ${definitions_linux} ${libs_path_linux} ${libs_linux} -o ${build_path}/test_bullet
	
