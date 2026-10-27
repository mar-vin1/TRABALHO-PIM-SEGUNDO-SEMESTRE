#include <stdio.h>
#include <stdlib.h>

#define MESES 12

void salvar(int *presencas, int *faltas, int workers, int mesIdx);
int lerInteiro(const char *msg, int min, int max);

int main(void) {
    int present;
    int opcao;
    int qtdfunc = 0;
    int diasUteis = 0;
    int mesIndex = 0;
    const char *mesNome[MESES] = {
        "Janeiro", "Fevereiro", "Março",  "Abril",
        "Maio",    "Junho",     "Julho",  "Agosto",
        "Setembro","Outubro",   "Novembro","Dezembro"
    };
    printf("=====================\n");
    printf("Relatório de presença\n");
    printf("=====================\n");
    qtdfunc = lerInteiro("Quantos funcionários trabalham na loja? ", 1, 100000);

    int *presentes = calloc(qtdfunc, sizeof(int));
    int *faltas    = calloc(qtdfunc, sizeof(int));
    if (!presentes || !faltas) {
        fprintf(stderr, "Erro: falha na alocação de memória.\n");
        free(presentes);
        free(faltas);
        return 1;
    }

    do {
        printf("\n== Menu ==\n"
               "1-Configurações iniciais\n"
               "2-Iniciar monitoramento\n"
               "3-Relatório do mês\n"
               "0-Sair\n");
        opcao = lerInteiro("Opção: ", 0, 3);

        switch (opcao) {
        case 1:
            for (int i = 0; i < MESES; i++)
                printf("%d-%s\n", i + 1, mesNome[i]);
            mesIndex = lerInteiro("Mês (1-12): ", 1, 12);
            printf("Mês escolhido: %s\n", mesNome[mesIndex - 1]);
            diasUteis = lerInteiro("Quantos dias úteis tem o mês? ", 1, 31);
            for (int k = 0; k < qtdfunc; k++) presentes[k] = faltas[k] = 0;
            break;

        case 2:
            if (diasUteis == 0) {
                printf("ERRO: acesse as configurações iniciais primeiro.\n");
                break;
            }
            printf("== Monitoramento do mês de %s ==\n", mesNome[mesIndex - 1]);
            for (int i = 0; i < diasUteis; i++) {
                printf("-- Dia útil nº %d --\n", i + 1);
                for (int j = 0; j < qtdfunc; j++) {
                    char msg[64];
                    snprintf(msg, sizeof msg,
                             "Funcionário nº %d compareceu? (1-Sim/0-Não): ", j + 1);
                    present = lerInteiro(msg, 0, 1);
                    if (present == 1) presentes[j]++;
                    else              faltas[j]++;
                }
            }
            printf("Fim do monitoramento do mês de %s\n", mesNome[mesIndex - 1]);
            printf("Acesse o relatório no menu para salvar os dados.\n");
            break;

        case 3:
            if (diasUteis == 0) {
                printf("ERRO: acesse as configurações iniciais primeiro.\n");
                break;
            }
            printf("== Relatório do mês de %s ==\n", mesNome[mesIndex - 1]);
            for (int i = 0; i < qtdfunc; i++) {
                printf("Funcionário nº %d: %d presença(s) e %d falta(s)\n",
                       i + 1, presentes[i], faltas[i]);
            }
            if (lerInteiro("Salvar em .txt? (1-Sim / 2-Não): ", 1, 2) == 1) {
                salvar(presentes, faltas, qtdfunc, mesIndex - 1);
                printf("Relatório salvo em 'Relatorio_mensal.txt'.\n");
            }
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    free(presentes);
    free(faltas);
    printf("Fim.\n");
    return 0;
}

int lerInteiro(const char *msg, int min, int max) {
    int v;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &v) == 1 && v >= min && v <= max) return v;
        printf("Valor inválido (esperado entre %d e %d).\n", min, max);
        while (getchar() != '\n');
    }
}

void salvar(int *presencas, int *faltas, int workers, int mesIdx) {
    static const char *mesNome[] = {
        "Janeiro", "Fevereiro", "Março",  "Abril",
        "Maio",    "Junho",     "Julho",  "Agosto",
        "Setembro","Outubro",   "Novembro","Dezembro"
    };

    FILE *fh = fopen("Relatorio_mensal.txt", "a");
    if (!fh) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo.\n");
        return;
    }
    fprintf(fh, "== Relatório do mês de %s ==\n", mesNome[mesIdx]);
    for (int i = 0; i < workers; i++) {
        fprintf(fh, "Funcionário %d: %d presença(s), %d falta(s)\n",
                i + 1, presencas[i], faltas[i]);
    }
    fprintf(fh, "\n");
    fclose(fh);
}