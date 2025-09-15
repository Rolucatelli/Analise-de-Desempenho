#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MINUTOS *60
#define HORAS *60 MINUTOS
#define DIAS *24 HORAS

double aleatorio()
{
    double u = rand() / ((double)RAND_MAX + 1);
    // u = [0,1)

    u = 1.0 - u;
    // u = (0,1]

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

void main()
{
    srand(time(NULL));

    // inicio
    double tempo_decorrido = 0.0; // 0 segundos

    // tempo total que desejo simular
    double tempo_simulacao = 1 DIAS; // 24 horas

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
            // acontece uma chegada
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
        }
        else
        {
            // acontece uma saida
            // printf("saida: %lF\n", tempo_decorrido);

            fila--;
            if (fila)
            {
                // atendo o proximo
                tempo_servico = tempo_decorrido + exponencial(media_tempo_servico);
                qtd_servicos++;
                soma_tempo_servico += tempo_servico - tempo_decorrido;
            }
        }
        // printf("fila: %ld\n========================\n", fila);
    }
    printf("\n\nqtd_requisicoes: %ld\n", qtd_requisicoes);
    printf("max_fila: %ld\n", max_fila);
    printf("media entre requisicoes: %lF\n", soma_inter_requisicoes / qtd_requisicoes);
    printf("media entre servicos: %lF\n", soma_tempo_servico / qtd_servicos);
}
