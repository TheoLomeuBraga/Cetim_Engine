import os
import platform

#configs

engine_cpp_path = "./src/Theo_Master_Peace/TMP.cpp"
font_reader_cpp_ui_path="./src/font_reader_gtk/font_reader_gtk.cpp"
includes="-I./src/Font_Reader -I./src/Theo_Master_Peace -I./src/Theo_Master_Peace_Core -I./include -I./include/freetype -I./include/bullet3 "
built_path="./built"

def run_commands(commands):
    command = ""
    for c in commands:
        command += c + " "
    os.system(command)
    print(command)

#windows

windows_config={
    "compiler": "g++",
    "tags": "-std=c++17 -pipe -Wa,-mbig-obj -static",
    "libs": "-llua -lglfw3  -lglew32 -lopengl32 -lbox2d -lfreetype -lsfml-audio-s -lsfml-network-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lBulletDynamics -lBulletCollision -lLinearMath",
    "libs_path": "-L./libs/windows",
    "definitions": "-DGLEW_STATIC -D_HAS_STD_BYTE=0 -DSFML_STATIC -DSFML_USE_STATIC_STD_LIBS -DAL_LIBTYPE_STATIC",
}


def build_windows():
    print("build windows")
    run_commands([windows_config["compiler"],windows_config["tags"],windows_config["libs"],windows_config["libs_path"],includes,windows_config["definitions"],engine_cpp_path,"-o " + built_path + "e/ngine_theo_master_peace.exe"])
    run_commands(["cp ./src/font_reader_gtk/font_reader_gtk.glade " + built_path + "/font_reader_gtk.glade"])
    run_commands([windows_config["compiler"],windows_config["tags"],windows_config["libs"],"`pkg-config --cflags --libs gtk+-3.0`",windows_config["libs_path"],includes,windows_config["definitions"],font_reader_cpp_ui_path,"-o " + built_path + "/font_reader_gtk.exe"])
    print("finished")

#linux

linux_config={
    "compiler": "g++",
    "tags": "-std=c++17",
    "libs": "-llua -lglfw -lGLEW -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lBulletDynamics -lBulletCollision -lLinearMath ",
    "libs_path": "-L./libs/linux",
    "definitions": "-DLINUX",
}

def build_linux():
    print("build linux")
    run_commands([linux_config["compiler"],linux_config["tags"],linux_config["libs"],linux_config["libs_path"],includes,linux_config["definitions"],engine_cpp_path,"-o " + built_path + "e/ngine_theo_master_peace"])
    run_commands(["cp ./src/font_reader_gtk/font_reader_gtk.glade " + built_path + "/font_reader_gtk.glade"])
    run_commands([linux_config["compiler"],linux_config["tags"],linux_config["libs"],"`pkg-config --cflags --libs gtk+-3.0`",linux_config["libs_path"],includes,linux_config["definitions"],font_reader_cpp_ui_path,"-o " + built_path + "/font_reader_gtk"])
    print("finished")








def build():
    os = platform.system()
    if os == "Windows":
        build_windows()

    if os == "Linux":
        build_linux()

build()




