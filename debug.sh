export LD_LIBRARY_PATH=$(pwd)/libs/linux
gdb $(pwd)/build/engine_theo_master_pice
c
y
set auto-solib-add off
run $(pwd)/projects/HMTF
c
y
backtrace

#windows
gdb $(pwd)/build/engine_theo_master_pice
c
set auto-solib-add off
run C:\Theo_Master_Peace\projects\HMTF
c
y
backtrace