# Nome do projeto
PROJ_NAME=heuristicas_3-partition

# Nome do arquivo de Debug
DEBUG_NAME=heuristicas_3-partition_debug

# Arquivos .c
C_SOURCE=$(wildcard ./src/*.c)

# Arquivos .h
H_SOURCE=$(wildcard ./hdr/*.h)

# Arquivos objeto
OBJ=$(subst .c,.o,$(subst src,objects,$(C_SOURCE) ./objects/main.c))

# Compilador
CC=gcc

# Flags (opções) para o compilador
CC_FLAGS=-c -W -Wall -pedantic

# Comando utilizado como target do clean
RM = rm -f

#########################
# Compilação e linkagem #
#########################
all: exeFolder $(PROJ_NAME)
debug: exeFolder $(DEBUG_NAME)

run: all
	@ ./exe/${PROJ_NAME}

create: exeFolder
	@ touch ./src/main.c


$(DEBUG_NAME): $(OBJ)
	@ echo 'Construindo arquivo de Debug usando o linker GCC: $@'
	$(CC) $^ -o ./exe/$@ -g
	@ echo 'Arquivo de Debug pronto!: $@'
	@ echo ' '

$(PROJ_NAME): $(OBJ)
	@ echo 'Construindo o binário usando o linker GCC: $@'
	$(CC) $^ -o ./exe/$@
	@ echo 'Binário pronto: $@'
	@ echo ' '

./objects/%.o: ./src/%.c ./hdr/%.h
	@ echo 'Construindo target usando o compilador GCC: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

exeFolder: objectsFolder
	@ mkdir -p exe 

objectsFolder: srcFolder hdrFolder
	@ mkdir -p objects

srcFolder:
	@ mkdir -p src

hdrFolder:
	@ mkdir -p hdr

clean:
	@ $(RM) ./objects/* ./exe/* *~

summary:
	@ summary 'Compilando e linkando o projeto: $(PROJ_NAME)'
	@ echo 'Arquivos .c: $(C_SOURCE)'
	@ echo 'Arquivos .h: $(H_SOURCE)'
	@ echo 'Arquivos .o: $(OBJ)'
	@ echo ' '

.PHONY: all clean debug run echo
# Se existir algum arquivo com o mesmo nome que uma tag, como clean.c ou all.c, ele ignora