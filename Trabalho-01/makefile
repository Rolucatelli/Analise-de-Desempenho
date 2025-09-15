ARQUIVO=simulacao_v2

all: exeFolder
	@ gcc -o ./exe/$(ARQUIVO) $(ARQUIVO).c -lm

run: all
	@ ./exe/$(ARQUIVO)

test: all
	@ ./exe/$(ARQUIVO) relatorio_80.csv 0.5 0.4
	@ ./exe/$(ARQUIVO) relatorio_90.csv 0.5 0.45
	@ ./exe/$(ARQUIVO) relatorio_95.csv 0.5 0.475
	@ ./exe/$(ARQUIVO) relatorio_99_9.csv 0.5 0.4995

exeFolder:
	@ mkdir -p exe