# TP2SO

# Compilar el SO
cd Toolchain
make all
cd ..
make clean
make all

# Correr el SO
./run.sh

# Progreso
Hicimos un memory manager tipo bitmap y un scheduler sin prioridades (round robbin), implementamos 
las syscalls :
    -createProcess
    -sleep
    -exit (destroy process)
    -ps
    -yield (ceder el CPU)

Todavia no corrimos los tests que paso la catedra, pero para testear funcionamiento de lo que hicimos, se puede hacer lo siguiente:
    una vez iniciada la shell, se puede usar el comando ps para listar los procesos, en donde se vera que hay uno solo. Con el comando dummy se pueden crear procesos que hacen un sleep de 5 segundos en background, permitiendo que en este intervalo se pueda hacer un ps y notar que hay un proceso adicional y que el mismo esta bloqueado. Luego de dicho intervalo de 5 segundos, ejecutando de vuelta un ps se puede ver que el proceso esta "DEAD". Como cada proceso nuevo creado utiliza el memory manager para alocar memoria, estamos verificando que esta funcionando correctamente.