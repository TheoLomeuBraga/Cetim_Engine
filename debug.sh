export LD_LIBRARY_PATH=$(pwd)/libs/linux
gdb $(pwd)/build/engine_theo_master_pice
c
y
set auto-solib-add off
run $(pwd)/projects/HMTF
c
y
backtrace

gprof -p $(pwd)/build/engine_theo_master_pice $(pwd)/projects/HMTF/gmon.out > relatorio.txt





export PATH="$HOME/.local/bin:$PATH"
gprof $(pwd)/build/engine_theo_master_pice | gprof2dot | dot -Tpng -o output.png


#windows
$(pwd)/build/engine_theo_master_pice.exe C:\\Theo_Master_Pice\\projects\\HMTF

gdb $(pwd)/build/engine_theo_master_pice
c
set auto-solib-add off
run C:\Theo_Master_Peace\projects\HMTF
c
y
backtrace