gdb $(pwd)/build/engine_theo_master_pice 
c
y
set auto-solib-add off
run $(pwd)/projects/HMTF
c
y
backtrace