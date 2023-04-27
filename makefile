# ORQUESTADOR DE MAKES

# COLORES PARA LA CONSOLA
RED :=\033[0;31m
GREEN :=\033[92m
YELLOW :=\033[0;33m
NOCOLOR :=\033[0m

# Me guardo esto por si a futuro nos sirve de algo como barra de carga
# ▓▓▓░░░░░░░░░░░░░░░░░

SHARED_NAME := SHARED
SHARED_ROUTE := shared/tests

# CONSOLA INFO
CONSOLA_NAME := CONSOLA
CONSOLA_ROUTE := consola/src
CONSOLA_TEST_ROUTE := consola/tests
CONSOLA_COMPILADO := consola.out
CONSOLA_ARCHIVO_CONFIG := consola.config

# KERNEL INFO
KERNEL_NAME := KERNEL
KERNEL_ROUTE := kernel/src
KERNEL_TEST_ROUTE := kernel/tests
KERNEL_COMPILADO := kernel.out
KERNEL_ARCHIVO_CONFIG := kernel.config

# CPU INFO
CPU_NAME := CPU
CPU_ROUTE := cpu/src
CPU_TEST_ROUTE := cpu/tests
CPU_COMPILADO := cpu.out
CPU_ARCHIVO_CONFIG := cpu.config

# MEMORIA INFO
MEMORIA_NAME := MEMORIA
MEMORIA_ROUTE := memoria/src
MEMORIA_TEST_ROUTE := memoria/tests
MEMORIA_COMPILADO := memoria.out
MEMORIA_ARCHIVO_CONFIG := memoria.config

# co = consola, m = memoria, k = kernel, cpu = cpu
all: co m k cpu
	
# Trabajos de compilacion proyectos y test "make d" "make r" "make m"
co:
	-@echo "$(YELLOW) COMPILANDO '$(CONSOLA_NAME)' 🎮 🎮 🎮 $(NOCOLOR)"
	-@cd $(CONSOLA_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) COMPILANDO TESTS '$(CONSOLA_NAME)' 🎮 🎮 🎮 $(NOCOLOR)"
	-@cd $(CONSOLA_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(CONSOLA_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"
	
cpu:
	-@echo "$(YELLOW) COMPILANDO '$(CPU_NAME)' 🎮 🎮 🎮 $(NOCOLOR)"
	-@cd $(CPU_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) COMPILANDO TESTS '$(CPU_NAME)' 🎮 🎮 🎮 $(NOCOLOR)"
	-@cd $(CPU_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(CPU_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"

k:
	-@echo "$(YELLOW) COMPILANDO '$(KERNEL_NAME)' 📝 📝 📝$(NOCOLOR)"
	-@cd $(KERNEL_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(KERNEL_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"

m:
	-@echo "$(YELLOW) COMPILANDO '$(MEMORIA_NAME)' 📝 📝 📝$(NOCOLOR)"
	-@cd $(MEMORIA_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) COMPILANDO TESTS '$(MEMORIA_NAME)' 📝 📝 📝 $(NOCOLOR)"
	-@cd $(MEMORIA_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(MEMORIA_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"

# Trabajos de clean "make clean-co" "make clean-cpu" "make clean-k" "make clean-m" "make clean-s" "make-clean"
clean-co:
	-@cd $(CONSOLA_ROUTE) && $(MAKE) clean
	-@cd $(CONSOLA_TEST_ROUTE) && $(MAKE) clean
	
clean-cpu:
	-@cd $(CPU_ROUTE) && $(MAKE) clean
	-@cd $(CPU_TEST_ROUTE) && $(MAKE) clean
	
clean-k:
	-@cd $(KERNEL_ROUTE) && $(MAKE) clean
	-@cd $(KERNEL_TEST_ROUTE) && $(MAKE) clean
	
clean-m:
	-@cd $(MEMORIA_ROUTE) && $(MAKE) clean
	-@cd $(MEMORIA_TEST_ROUTE) && $(MAKE) clean

clean-s:
	-@cd $(SHARED_ROUTE) && $(MAKE) clean

clean: clean-co clean-cpu clean-k clean-m clean-s
	-@find . -name "*.o" -type f -delete
	-@clear
	-@echo "$(GREEN) 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️   Limpiado archvos 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️  $(NOCOLOR)"

clear: clean
	-@echo "$(YELLOW) Sabía que no te acordarías que el comando es 'clean' no 'clear' así que lo codee igualmente $(NOCOLOR)"

# Trabajos para correr los tests "make test-d" "make test-r" "make test-m" "make test-s" "make test"
test-k:
	-@cd $(KERNEL_TEST_ROUTE) && $(MAKE) test

test-co:
	-@cd $(CONSOLA_TEST_ROUTE) && $(MAKE) test
	
test-m:
	-@cd $(MEMORIA_TEST_ROUTE) && $(MAKE) test
	
test-cpu:
	-@cd $(CPU_TEST_ROUTE) && $(MAKE) test

test-s:
	-@cd $(SHARED_ROUTE) && $(MAKE) test
	
test: test-k test-co test-m test-cpu test-s
	-@echo "$(YELLOW) Ejecutados todos los tests $(NOCOLOR)"
	
# Trabajos para correr los procesos con valgrind "make valgrind-s" "make valgrind-d" "make valgrind-r" "make valgrind-m"
valgrind-s:
	-@cd $(SHARED_ROUTE) && $(MAKE) valgrind

valgrind-co:
	-@cd $(CONSOLA_ROUTE) && $(MAKE) valgrind
	
valgrind-cpu:
	-@cd $(CPU_ROUTE) && $(MAKE) valgrind
		
valgrind-k:
	-@cd $(KERNEL_ROUTE) && $(MAKE) valgrind

valgrind-m:
	-@cd $(MEMORIA_ROUTE) && $(MAKE) valgrind

# Trabajos para ejecutar los archivos compilados "make run-d" "make run-r" "make run-m"
run-co:
	-@./$(CONSOLA_COMPILADO) $(CONSOLA_ARCHIVO_CONFIG)
	
run-cpu:
	-@./$(CPU_COMPILADO) $(CPU_ARCHIVO_CONFIG)

run-k:
	-@./$(KERNEL_COMPILADO) $(KERNEL_ARCHIVO_CONFIG)

run-m:
	-@./$(MEMORIA_COMPILADO) $(MEMORIA_ARCHIVO_CONFIG)

.PHONY: all test clean consola cpu kernel memoria shared
