export LD_LIBRARY_PATH=$(pwd)/libs/linux
gdb $(pwd)/build/cetim_engine
c
y
set auto-solib-add off
run $(pwd)/projects/HMTF
c
y
backtrace

gprof -p $(pwd)/build/cetim_engine $(pwd)/projects/HMTF/gmon.out > relatorio.txt





export PATH="$HOME/.local/bin:$PATH"
gprof $(pwd)/build/cetim_engine | gprof2dot | dot -Tpng -o output.png


#windows
$(pwd)/build/cetim_engine.exe C:\\Cetim_Engine\\projects\\HMTF

gdb $(pwd)/build/cetim_engine.exe
c
set auto-solib-add off
run C:\Cetim_Engine\projects\HMTF
c
y
backtrace