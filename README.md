# tp-2022-1c-Operemas-Sistemativos

## Para compilar los módulos sobre consola, nos tenemos que posicionar sobre la carpeta de tp, y ejecutar lo siguiente:
- make all -- para compilar todos los módulos;
- make co -- solo consola
- make cpu -- solo compilar
- make k -- solo kernel
- make m -- solo memoria

## Para ejecutar los módulos, lo hacemos uno por uno con el siguiente comando:
- make run-co -- ejecuta consola
- make run-cpu -- ejecuta cpu
- make run-k -- ejecuta kernel
- make run-m -- ejecuta memoria

## Para ejecutar el programa con valgrind para checkear memory leaks, debemos hacer:
- make valgrind-co
- make valgrind-cpu
- make valgrind-k
- make valgrind-m

## Para limpiar los archivos, podemos ejecutar:
- make clean
