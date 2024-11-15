// calculadora.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>

#define TAM_MAX_EXPRESSAO 1024

// Estrutura para nós da árvore de sintaxe abstrata
typedef struct No {
    char tipo; // 'N' para número, 'O' para operador, 'F' para função
    double valor;
    char operador;
    char funcao[5]; // Nome da função (ex: "sin", "cos", etc.)
    struct No *esquerda;
    struct No *direita;
} No;

// Prototipação das funções
No* analisarExpressao(char **str);
No* analisarTermo(char **str);
No* analisarFator(char **str);
No* analisarPotencia(char **str);
No* analisarUnario(char **str);
No* analisarPrimario(char **str);
No* analisarNumeroOuConstante(char **str);
No* analisarFuncao(char **str, char *nomeFuncao);
void pularEspacos(char **str);
double avaliar(No *no, int *passo);
double fatorial(double n);
void liberarArvore(No *no);
void imprimirInfixo(No *no);

int main() {
    setlocale(LC_ALL, "");
    printf("Bem-vindo à Calculadora!\n");
    printf("Guia:\n");
    printf("- Suporta operações: +, -, *, /, ^, sqrt, %%, !\n");
    printf("- Suporta funções: sin, cos, tan, ln, sqrt\n");
    printf("- Suporta constantes: pi, e\n");
    printf("- Suporta agrupamentos: (), [], {}\n");
    printf("- Use 'deg' apos um numero para indicar graus em funcoes trigonometricas.\n");
    printf("- Suporta multiplicaçãoo implícita (ex: 2(3) = 2 * 3)\n");
    printf("- O símbolo '%%' é usado como operador de porcentagem (ex: 50%% = 0.5)\n");
    printf("- Exemplo de expressão: 3+sin(45deg)*3-(32+[5+1]*2(3/5))^3-sqrt(9)+{3!}+(50%%*1000)\n\n");

    char expressao[TAM_MAX_EXPRESSAO];
    printf("Digite a expressão matemática:\n");
    if (fgets(expressao, sizeof(expressao), stdin) == NULL) {
        printf("Erro na leitura da expressão.\n");
        return 1;
    }

    // Remove o caractere de nova linha, se presente
    size_t len = strlen(expressao);
    if (len > 0 && expressao[len - 1] == '\n') {
        expressao[len - 1] = '\0';
    }

    char *p = expressao;
    No *arvore = analisarExpressao(&p);

    // Verifica se toda a expressao foi consumida
    pularEspacos(&p);
    if (*p != '\0') {
        printf("Erro: Expressão invalida ou caracteres inesperados no final.\n");
        liberarArvore(arvore);
        return 1;
    }

    printf("\nPasso a Passo da Resolução:\n");

    int passo = 1;
    double resultado = avaliar(arvore, &passo);
    printf("\nResultado Final: %.5f\n", resultado);

    liberarArvore(arvore);
    return 0;
}

// Funcoes de analise sintatica (parsing)
void pularEspacos(char **str) {
    while (isspace(**str)) {
        (*str)++;
    }
}

No* analisarExpressao(char **str) {
    No *no = analisarTermo(str);
    while (1) {
        pularEspacos(str);
        char op = **str;
        if (op == '+' || op == '-') {
            (*str)++;
            No *direita = analisarTermo(str);
            No *novoNo = malloc(sizeof(No));
            novoNo->tipo = 'O';
            novoNo->operador = op;
            novoNo->esquerda = no;
            novoNo->direita = direita;
            no = novoNo;
        } else {
            break;
        }
    }
    return no;
}

No* analisarTermo(char **str) {
    No *no = analisarFator(str);
    while (1) {
        pularEspacos(str);

        // Suporte para multiplicacao implicita
        if ((**str == '(' || **str == '[' || **str == '{') ||
            isalpha(**str) || isdigit(**str) || **str == '.') {
            // Inserir operador de multiplicacao implicito
            No *direita = analisarFator(str);
            No *novoNo = malloc(sizeof(No));
            novoNo->tipo = 'O';
            novoNo->operador = '*';
            novoNo->esquerda = no;
            novoNo->direita = direita;
            no = novoNo;
            continue;
        }

        char op = **str;
        if (op == '*' || op == '/' ) {
            (*str)++;
            No *direita = analisarFator(str);
            No *novoNo = malloc(sizeof(No));
            novoNo->tipo = 'O';
            novoNo->operador = op;
            novoNo->esquerda = no;
            novoNo->direita = direita;
            no = novoNo;
        } else {
            break;
        }
    }
    return no;
}

No* analisarFator(char **str) {
    No *no = analisarPotencia(str);
    while (1) {
        pularEspacos(str);
        if (**str == '^') {
            (*str)++;
            No *direita = analisarPotencia(str);
            No *novoNo = malloc(sizeof(No));
            novoNo->tipo = 'O';
            novoNo->operador = '^';
            novoNo->esquerda = no;
            novoNo->direita = direita;
            no = novoNo;
        } else {
            break;
        }
    }
    return no;
}

No* analisarPotencia(char **str) {
    return analisarUnario(str);
}

No* analisarUnario(char **str) {
    pularEspacos(str);
    char op = **str;
    if (op == '+' || op == '-') {
        (*str)++;
        No *direita = analisarUnario(str);
        No *novoNo = malloc(sizeof(No));
        novoNo->tipo = 'O';
        novoNo->operador = op;
        novoNo->esquerda = NULL; // Operador unario
        novoNo->direita = direita;
        return novoNo;
    }
    return analisarPrimario(str);
}

No* analisarPrimario(char **str) {
    pularEspacos(str);
    char op = **str;

    // Verifica se é uma função ou constante
    if (isalpha(op)) {
        // Funções: sin, cos, tan, ln, sqrt
        // Constantes: pi, e
        char buffer[5] = {0};
        int i = 0;
        while (isalpha(**str) && i < 4) { // Máximo 4 caracteres
            buffer[i++] = **str;
            (*str)++;
        }
        buffer[i] = '\0';

        if (strcmp(buffer, "sin") == 0 || strcmp(buffer, "cos") == 0 ||
            strcmp(buffer, "tan") == 0 || strcmp(buffer, "ln") == 0 ||
            strcmp(buffer, "sqrt") == 0) {
            return analisarFuncao(str, buffer);
        } else if (strcmp(buffer, "pi") == 0) {
            No *no = malloc(sizeof(No));
            no->tipo = 'N';
            no->valor = M_PI;
            no->esquerda = no->direita = NULL;
            return no;
        } else if (strcmp(buffer, "e") == 0) {
            No *no = malloc(sizeof(No));
            no->tipo = 'N';
            no->valor = M_E;
            no->esquerda = no->direita = NULL;
            return no;
        } else {
            printf("Erro: Funcao ou constante desconhecida '%s'.\n", buffer);
            exit(1);
        }
    }

    if (op == '(' || op == '[' || op == '{') {
        char abre = op;
        char fecha = (op == '(') ? ')' : (op == '[') ? ']' : '}';
        (*str)++;
        No *no = analisarExpressao(str);
        pularEspacos(str);
        if (**str == fecha) {
            (*str)++;
        } else {
            printf("Erro: Parenteses '%c' nao fechados corretamente.\n", abre);
            liberarArvore(no);
            exit(1);
        }

        // Verifica se há um fatorial após os parênteses
        pularEspacos(str);
        if (**str == '!') {
            (*str)++;
            No *noFatorial = malloc(sizeof(No));
            noFatorial->tipo = 'O';
            noFatorial->operador = '!';
            noFatorial->esquerda = no;
            noFatorial->direita = NULL;
            return noFatorial;
        }

        return no;
    } else if (isdigit(op) || op == '.') {
        return analisarNumeroOuConstante(str);
    } else {
        printf("Erro: Simbolo inesperado '%c'.\n", op);
        exit(1);
    }
}

No* analisarNumeroOuConstante(char **str) {
    pularEspacos(str);
    char *inicio = *str;
    while (isdigit(**str) || **str == '.') {
        (*str)++;
    }

    char numeroStr[64];
    int len = *str - inicio;
    strncpy(numeroStr, inicio, len);
    numeroStr[len] = '\0';
    double valor = atof(numeroStr);

    // Verifica se há 'deg' para indicar graus
    pularEspacos(str);
    if (strncmp(*str, "deg", 3) == 0) {
        *str += 3; // Avança o ponteiro em 3 posições
        valor = valor * M_PI / 180.0; // Converte para radianos
    }

    // Verifica se há o símbolo de porcentagem '%'
    if (**str == '%') {
        (*str)++;
        valor = valor / 100.0; // Converte para fração decimal
    }

    No *no = malloc(sizeof(No));
    no->tipo = 'N';
    no->valor = valor;
    no->esquerda = no->direita = NULL;

    // Verifica se há um fatorial '!' após o número
    pularEspacos(str);
    if (**str == '!') {
        (*str)++;
        No *noFatorial = malloc(sizeof(No));
        noFatorial->tipo = 'O';
        noFatorial->operador = '!';
        noFatorial->esquerda = no;
        noFatorial->direita = NULL;
        return noFatorial;
    }

    return no;
}

No* analisarFuncao(char **str, char *nomeFuncao) {
    pularEspacos(str);
    if (**str == '(') {
        (*str)++;
        No *argumento = analisarExpressao(str);
        pularEspacos(str);
        if (**str == ')') {
            (*str)++;
        } else {
            printf("Erro: Parenteses ')' nao fechados corretamente na funcao '%s'.\n", nomeFuncao);
            liberarArvore(argumento);
            exit(1);
        }
        No *noFuncao = malloc(sizeof(No));
        noFuncao->tipo = 'F';
        strcpy(noFuncao->funcao, nomeFuncao);
        noFuncao->esquerda = argumento;
        noFuncao->direita = NULL;
        return noFuncao;
    } else {
        printf("Erro: Esperado '(' apos a funcao '%s'.\n", nomeFuncao);
        exit(1);
    }
}

double avaliar(No *no, int *passo) {
    if (no->tipo == 'N') {
        return no->valor;
    } else if (no->tipo == 'F') {
        double arg = avaliar(no->esquerda, passo);
        double resultado = 0;
        if (strcmp(no->funcao, "sin") == 0) {
            resultado = sin(arg);
        } else if (strcmp(no->funcao, "cos") == 0) {
            resultado = cos(arg);
        } else if (strcmp(no->funcao, "tan") == 0) {
            resultado = tan(arg);
        } else if (strcmp(no->funcao, "ln") == 0) {
            if (arg <= 0) {
                printf("Erro: Logaritmo de numero nao positivo.\n");
                exit(1);
            }
            resultado = log(arg);
        } else if (strcmp(no->funcao, "sqrt") == 0) {
            if (arg < 0) {
                printf("Erro: Raiz quadrada de numero negativo.\n");
                exit(1);
            }
            resultado = sqrt(arg);
        } else {
            printf("Erro: Funcao desconhecida '%s'.\n", no->funcao);
            exit(1);
        }
        printf("%d. %s(%.5f) = %.5f\n", (*passo)++, no->funcao, arg, resultado);
        no->tipo = 'N';
        no->valor = resultado;
        liberarArvore(no->esquerda);
        no->esquerda = no->direita = NULL;
        return resultado;
    } else if (no->tipo == 'O') {
        double esquerda = 0, direita = 0;
        if (no->esquerda) esquerda = avaliar(no->esquerda, passo);
        if (no->direita) direita = avaliar(no->direita, passo);
        double resultado = 0;

        switch (no->operador) {
            case '+': resultado = esquerda + direita; break;
            case '-': resultado = esquerda - direita; break;
            case '*': resultado = esquerda * direita; break;
            case '/':
                if (direita == 0) {
                    printf("Erro: Divisao por zero.\n");
                    exit(1);
                }
                resultado = esquerda / direita;
                break;
            case '^': resultado = pow(esquerda, direita); break;
            case '!':
                if (esquerda < 0 || floor(esquerda) != esquerda) {
                    printf("Erro: Fatorial indefinido para numeros negativos ou nao inteiros.\n");
                    exit(1);
                }
                resultado = fatorial(esquerda);
                break;
            default:
                printf("Erro: Operador desconhecido '%c'.\n", no->operador);
                exit(1);
        }
        printf("%d. ", (*passo)++);
        imprimirInfixo(no);
        printf(" = %.5f\n", resultado);
        no->tipo = 'N';
        no->valor = resultado;
        liberarArvore(no->esquerda);
        liberarArvore(no->direita);
        no->esquerda = no->direita = NULL;
        return resultado;
    }
    return 0;
}

void imprimirInfixo(No *no) {
    if (no == NULL) return;
    if (no->tipo == 'F') {
        printf("%s(", no->funcao);
        imprimirInfixo(no->esquerda);
        printf(")");
    } else if (no->tipo == 'O') {
        if (no->operador == '!') {
            imprimirInfixo(no->esquerda);
            printf("!");
        } else {
            printf("(");
            imprimirInfixo(no->esquerda);
            printf(" %c ", no->operador);
            imprimirInfixo(no->direita);
            printf(")");
        }
    } else if (no->tipo == 'N') {
        printf("%.5f", no->valor);
    }
}

double fatorial(double n) {
    if (n == 0 || n == 1) return 1;
    double resultado = 1;
    for (int i = 2; i <= (int)n; i++) {
        resultado *= i;
    }
    return resultado;
}

void liberarArvore(No *no) {
    if (no == NULL) return;
    liberarArvore(no->esquerda);
    liberarArvore(no->direita);
    free(no);
}
