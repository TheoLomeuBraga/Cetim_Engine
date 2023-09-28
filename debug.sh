export LD_LIBRARY_PATH=$(pwd)/libs/linux
gdb $(pwd)/build/engine_theo_master_pice
c
y
set auto-solib-add off
run $(pwd)/projects/HMTF
c
y
backtrace

gprof -p $(pwd)/build/engine_theo_master_pice gmon.out > relatorio.txt
grep -E "^[[:space:]]+[0-9]+.[0-9]+:[[:space:]]+[0-9]+[[:space:]]+[[:digit:]]+" relatorio.txt | awk '{print $NF}' > relatorio_limpo.txt





export PATH="$HOME/.local/bin:$PATH"
gprof $(pwd)/build/engine_theo_master_pice | gprof2dot | dot -Tpng -o output.png


#windows
gdb $(pwd)/build/engine_theo_master_pice
c
set auto-solib-add off
run C:\Theo_Master_Peace\projects\HMTF
c
y
backtrace