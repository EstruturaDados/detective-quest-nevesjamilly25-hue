#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura que representa uma sala (nó da árvore binária)
 */
typedef struct Sala {
    char *nome;            // nome da sala (alocado dinamicamente)
    struct Sala *esquerda; // ponteiro para caminho à esquerda
    struct Sala *direita;  // ponteiro para caminho à direita
} Sala;

/*
 * criarSala:
 *   - cria dinamicamente um nó (Sala) com o nome informado.
 *   - retorna ponteiro para a Sala criada.
 */
Sala *criarSala(const char *nome) {
    Sala *nova = (Sala *)malloc(sizeof(Sala));
    if (!nova) {
        fprintf(stderr, "Erro: falha na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    size_t len = strlen(nome) + 1;
    nova->nome = (char *)malloc(len);
    if (!nova->nome) {
        fprintf(stderr, "Erro: falha na alocação de memória para nome.\n");
        free(nova);
        exit(EXIT_FAILURE);
    }
    strncpy(nova->nome, nome, len);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/*
 * explorarSalas:
 *   - recebe a raiz da árvore (sala inicial) e permite ao jogador navegar.
 *   - escolhas: 'e' (esquerda), 'd' (direita), 's' (sair).
 *   - exibe cada sala visitada e ao final mostra o caminho percorrido.
 */
void explorarSalas(Sala *raiz) {
    if (!raiz) {
        printf("Mapa vazio. Não há salas para explorar.\n");
        return;
    }

    // vetor simples para armazenar as salas visitadas (apontadores para nomes)
    const char *visitadas[100];
    int cont = 0;

    Sala *atual = raiz;
    char escolha;

    printf("Bem-vindo(a) a Detective Quest — Exploração da Mansão!\n");
    printf("Você começará pelo Hall de entrada.\n\n");

    // marca a primeira sala como visitada e exibe
    visitadas[cont++] = atual->nome;
    printf("Você está em: %s\n", atual->nome);

    // enquanto houver caminhos ou até o usuário sair
    while (1) {
        // se for nó-folha (sem caminhos), encerra automaticamente
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou a um cômodo sem mais caminhos (%s).\n", atual->nome);
            break;
        }

        // instruções ao jogador
        printf("\nEscolha o caminho: (e) esquerda");
        if (atual->direita) printf(" | (d) direita");
        printf(" | (s) sair\n");
        printf("Digite sua escolha: ");

        // lê a escolha ignorando espaços em branco
        if (scanf(" %c", &escolha) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            // consumir resto da linha
            int c; while ((c = getchar()) != '\n' && c != EOF) ;
            continue;
        }

        // tratar opções
        if (escolha == 's' || escolha == 'S') {
            printf("Saindo da exploração por solicitação do usuário.\n");
            break;
        } else if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) {
                atual = atual->esquerda;
                visitadas[cont++] = atual->nome;
                printf("Você foi para: %s\n", atual->nome);
            } else {
                printf("Não há caminho para a esquerda a partir de %s.\n", atual->nome);
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) {
                atual = atual->direita;
                visitadas[cont++] = atual->nome;
                printf("Você foi para: %s\n", atual->nome);
            } else {
                printf("Não há caminho para a direita a partir de %s.\n", atual->nome);
            }
        } else {
            printf("Opção inválida. Use 'e' (esquerda), 'd' (direita) ou 's' (sair).\n");
        }
    }

    // mostrar resumo do percurso
    printf("\n--- Resumo das salas visitadas ---\n");
    for (int i = 0; i < cont; ++i) {
        printf("%d) %s\n", i + 1, visitadas[i]);
    }
    printf("----------------------------------\n");
}

/*
 * liberarSala:
 *   - função recursiva para liberar toda a memória da árvore.
 *   - boa prática para não vazar memória.
 */
void liberarSala(Sala *no) {
    if (!no) return;
    liberarSala(no->esquerda);
    liberarSala(no->direita);
    free(no->nome);
    free(no);
}

/*
 * main:
 *   - monta manualmente a árvore (mapa da mansão) chamando criarSala()
 *   - inicia a exploração chamando explorarSalas()
 *   - libera memória antes de encerrar.
 *
 * A árvore abaixo é apenas um exemplo. Ajuste nomes/estrutura conforme desejar.
 */
int main(void) {
    // Montagem manual do mapa (árvore) — alocação dinâmica
    Sala *hall = criarSala("Hall de Entrada");

    // nível 1
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita  = criarSala("Biblioteca");

    // nível 2 - a partir de Sala de Estar
    hall->esquerda->esquerda = criarSala("Cozinha");
    hall->esquerda->direita  = criarSala("Sala de Jantar");

    // nível 2 - a partir de Biblioteca
    hall->direita->esquerda = criarSala("Escritório");
    hall->direita->direita  = criarSala("Observatório");

    // nível 3 - exemplo de folhas
    hall->esquerda->esquerda->esquerda = criarSala("Despensa"); // folha
    hall->direita->direita->direita = criarSala("Cúpula");      // folha

    // iniciar exploração interativa
    explorarSalas(hall);

    // limpar memória antes de sair
    liberarSala(hall);

    printf("\nExploração finalizada. Obrigado por jogar!\n");
    return 0;
}
