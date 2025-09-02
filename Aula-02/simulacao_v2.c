#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MINUTOS *60
#define HORAS *60 MINUTOS
#define DIAS *24 HORAS
#define MESES *30 DIAS
#define ANOS *12 MESES

typedef struct
{
    double tempo_anterior;
    unsigned long int qt_requisicoes;
    double soma_area;

} medida_little;

double aleatorio()
{
    double u = rand() / ((double)RAND_MAX + 1); // u entre [0,1)

    u = 1.0 - u; // u entre (0,1]

    return (u);
}

double exponencial(double l)
{
    return (-1.0 / l) * log(aleatorio());
}

double min(double a, double b)
{
    return a < b ? a : b;
}

void inicial_little(medida_little *medidas)
{
    medidas->tempo_anterior = 0.0;
    medidas->qt_requisicoes = 0.0;
    medidas->soma_area = 0;
}

void main()
{
    srand(time(NULL));

    /**
     * Declaracao little
     */
    medida_little E_N;
    medida_little E_W_chegadas, E_W_saidas;

    /**
     * iniciando variaveis de little
     */
    inicial_little(&E_N);
    inicial_little(&E_W_chegadas);
    inicial_little(&E_W_saidas);

    // inicio
    double tempo_decorrido = 0.0; // 0 segundos

    // tempo total que desejo simular
    double tempo_simulacao = 1.0 DIAS; // 24 horas

    // intervalos entre requisicoes
    double media_inter_requisicoes;

    // tempo medio gasto entre 1 atendimento
    double media_tempo_servico;

    // marca sempre o tempo de chegada da proxima requisicao
    double proxima_requisicao;

    // possui o tempo de servico da requisicao atualmente
    double tempo_servico;

    // fila
    unsigned long int fila = 0;
    unsigned long int max_fila = 0;

    // variáveis de medicao para validacao matematica
    unsigned long int qtd_requisicoes = 0;
    double soma_inter_requisicoes;
    unsigned long int qtd_servicos = 0;
    double soma_tempo_servico = 0.0;

    printf("Informe a media de tempo entre requisicoes: ");
    scanf("%lF", &media_inter_requisicoes);

    /*
        precisamos do valor do parametro l para gerar
        os numeros pseudo-aleatorios.
        lembre-se que l = 1.0/media
    */

    media_inter_requisicoes = 1.0 / media_inter_requisicoes;

    printf("Informe a media de tempo para atendimento: ");
    scanf("%lF", &media_tempo_servico);

    media_tempo_servico = 1.0 / media_tempo_servico;

    // gerando o tempo de chegada da primeira requisicao
    proxima_requisicao = exponencial(media_inter_requisicoes);

    qtd_requisicoes++;
    soma_inter_requisicoes = proxima_requisicao;
    while (tempo_decorrido < tempo_simulacao)
    {
        tempo_decorrido = fila ? min(proxima_requisicao, tempo_servico) : proxima_requisicao;

        // printf("tempo_decorido: %lF\n", tempo_decorrido);

        // tratando os eventos da simulacao:
        if (tempo_decorrido == proxima_requisicao)
        {
            // ACONTECE UMA CHEGADA
            // printf("chegada: %lF\n", tempo_decorrido);

            fila++;
            max_fila = fila > max_fila ? fila : max_fila;
            if (fila == 1)
            {
                // ambiente estava ocioso, inicia atendimento imediatamente
                tempo_servico = tempo_decorrido + exponencial(media_tempo_servico);
                qtd_servicos++;
                soma_tempo_servico += tempo_servico - tempo_decorrido;
            }
            proxima_requisicao = tempo_decorrido + exponencial(media_inter_requisicoes);

            qtd_requisicoes++;
            soma_inter_requisicoes += proxima_requisicao - tempo_decorrido;

            /**
             * little
             */

            E_N.soma_area += (tempo_decorrido - E_N.tempo_anterior) * E_N.qt_requisicoes;
            E_N.qt_requisicoes++;
            E_N.tempo_anterior = tempo_decorrido;

            E_W_chegadas.soma_area += (tempo_decorrido - E_W_chegadas.tempo_anterior) * E_W_chegadas.qt_requisicoes;
            E_W_chegadas.qt_requisicoes++;
            E_W_chegadas.tempo_anterior = tempo_decorrido;
        }
        else
        {
            // ACONTECE UMA SAIDA
            // printf("saida: %lF\n", tempo_decorrido);

            fila--;
            if (fila)
            {
                // atendo o proximo
                tempo_servico = tempo_decorrido + exponencial(media_tempo_servico);
                qtd_servicos++;
                soma_tempo_servico += tempo_servico - tempo_decorrido;
            }

            /**
             * little
             */

            E_N.soma_area += (tempo_decorrido - E_N.tempo_anterior) * E_N.qt_requisicoes;
            E_N.qt_requisicoes--;
            E_N.tempo_anterior = tempo_decorrido;

            E_W_saidas.soma_area += (tempo_decorrido - E_W_saidas.tempo_anterior) * E_W_saidas.qt_requisicoes;
            E_W_saidas.qt_requisicoes++;
            E_W_saidas.tempo_anterior = tempo_decorrido;
        }
        // printf("fila: %ld\n========================\n", fila);
    }
    printf("\n===============================\n");
    printf("     Metricas e validacoes     \n");
    printf("===============================\n");
    printf("qtd_requisicoes: %ld\n", qtd_requisicoes);
    printf("max_fila: %ld\n", max_fila);
    printf("media entre requisicoes: %lF\n", soma_inter_requisicoes / qtd_requisicoes);
    printf("media entre servicos: %lF\n", soma_tempo_servico / qtd_servicos);
    printf("\n===============================\n");
    printf("          Little\n");
    printf("===============================\n");

    double E_N_final = E_N.soma_area / tempo_decorrido;
    double E_W_final = (E_W_chegadas.soma_area - E_W_saidas.soma_area) / E_W_chegadas.qt_requisicoes;

    double lambda = (E_W_chegadas.qt_requisicoes / tempo_decorrido);
    double erro_little = E_N_final - lambda * E_W_final;

    printf("E[N]: %lF\n", E_N_final);
    printf("E[W]: %lF\n", E_W_final);
    printf("lambda: %lF\n", lambda);
    printf("Erro little: %lF\n", erro_little);

    printf("\n===============================\n");
    printf("          Ocupacao\n");
    printf("===============================\n");
    printf("ocupacao calculada: %lF\n", soma_tempo_servico / tempo_decorrido);
    printf("ocupacao esperada: %lF\n", media_inter_requisicoes / media_tempo_servico);
}
