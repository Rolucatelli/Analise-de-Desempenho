ARQUIVO=simulacao_v2

all: exeFolder
	@ gcc -o ./exe/$(ARQUIVO) $(ARQUIVO).c -lm

run: all
	@ ./exe/$(ARQUIVO)

exeFolder:
	@ mkdir -p exe