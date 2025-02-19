#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>

#define TAM_MAX_EXPRESSAO 1024

typedef struct No {
    char tipo;         // 'N' = número, 'O' = operador, 'F' = função
    double valor;
    char operador;
    char funcao[5];
    struct No *esquerda;
    struct No *direita;
} No;

No* analisarExpressao(char **str);
No* analisarTermo(char **str);
No* analisarFator(char **str);
No* analisarPotencia(char **str);
No* analisarUnario(char **str);
No* analisarPrimario(char **str);
No* analisarNumeroOuConstante(char **str);
No* analisarFuncao(char **str, char *nomeFuncao);
double avaliar(No *no, int *passo);
double fatorial(double n);
void imprimirInfixo(No *no);
void liberarArvore(No *no);
void pularEspacos(char **str);
void *safeMalloc(size_t size);

void *safeMalloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria.\n");
        exit(1);
    }
    return ptr;
}

void pularEspacos(char **str) {
    while (isspace(**str))
        (*str)++;
}

No* analisarExpressao(char **str) {
    No *no = analisarTermo(str);
    while (1) {
        pularEspacos(str);
        char op = **str;
        if (op == '+' || op == '-') {
            (*str)++;
            No *direita = analisarTermo(str);
            No *novoNo = safeMalloc(sizeof(No));
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
        if ((**str == '(' || **str == '[' || **str == '{') ||
            isalpha(**str) || isdigit(**str) || **str == '.') {
            No *direita = analisarFator(str);
            No *novoNo = safeMalloc(sizeof(No));
            novoNo->tipo = 'O';
            novoNo->operador = '*';
            novoNo->esquerda = no;
            novoNo->direita = direita;
            no = novoNo;
            continue;
        }
        char op = **str;
        if (op == '*' || op == '/') {
            (*str)++;
            No *direita = analisarFator(str);
            No *novoNo = safeMalloc(sizeof(No));
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
    pularEspacos(str);
    if (**str == '^') {
        (*str)++;
        No *novoNo = safeMalloc(sizeof(No));
        novoNo->tipo = 'O';
        novoNo->operador = '^';
        novoNo->esquerda = no;
        novoNo->direita = analisarFator(str);
        return novoNo;
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
        No *novoNo = safeMalloc(sizeof(No));
        novoNo->tipo = 'O';
        novoNo->operador = op;
        novoNo->esquerda = NULL;
        novoNo->direita = direita;
        return novoNo;
    }
    return analisarPrimario(str);
}

No* analisarPrimario(char **str) {
    pularEspacos(str);
    char op = **str;
    if (isalpha(op)) {
        char buffer[5] = {0};
        int i = 0;
        while (isalpha(**str) && i < 4) {
            buffer[i++] = **str;
            (*str)++;
        }
        buffer[i] = '\0';
        if (strcmp(buffer, "sin") == 0 || strcmp(buffer, "cos") == 0 ||
            strcmp(buffer, "tan") == 0 || strcmp(buffer, "ln") == 0 ||
            strcmp(buffer, "sqrt") == 0)
            return analisarFuncao(str, buffer);
        else if (strcmp(buffer, "pi") == 0) {
            No *no = safeMalloc(sizeof(No));
            no->tipo = 'N';
            no->valor = M_PI;
            no->esquerda = no->direita = NULL;
            return no;
        } else if (strcmp(buffer, "e") == 0) {
            No *no = safeMalloc(sizeof(No));
            no->tipo = 'N';
            no->valor = M_E;
            no->esquerda = no->direita = NULL;
            return no;
        } else {
            fprintf(stderr, "Erro: Funcao ou constante desconhecida '%s'.\n", buffer);
            exit(1);
        }
    }
    if (op == '(' || op == '[' || op == '{') {
        char abre = op, fecha = (op == '(') ? ')' : (op == '[') ? ']' : '}';
        (*str)++;
        No *no = analisarExpressao(str);
        pularEspacos(str);
        if (**str == fecha)
            (*str)++;
        else {
            fprintf(stderr, "Erro: Parenteses '%c' nao fechados corretamente.\n", abre);
            liberarArvore(no);
            exit(1);
        }
        pularEspacos(str);
        if (**str == '!') {
            (*str)++;
            No *noFatorial = safeMalloc(sizeof(No));
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
        fprintf(stderr, "Erro: Simbolo inesperado '%c'.\n", op);
        exit(1);
    }
}

No* analisarNumeroOuConstante(char **str) {
    pularEspacos(str);
    char *inicio = *str;
    while (isdigit(**str) || **str == '.')
        (*str)++;
    char numeroStr[64];
    int len = *str - inicio;
    strncpy(numeroStr, inicio, len);
    numeroStr[len] = '\0';
    double valor = atof(numeroStr);
    pularEspacos(str);
    if (strncmp(*str, "deg", 3) == 0) {
        *str += 3;
        valor = valor * M_PI / 180.0;
    }
    if (**str == '%') {
        (*str)++;
        valor /= 100.0;
    }
    No *no = safeMalloc(sizeof(No));
    no->tipo = 'N';
    no->valor = valor;
    no->esquerda = no->direita = NULL;
    pularEspacos(str);
    if (**str == '!') {
        (*str)++;
        No *noFatorial = safeMalloc(sizeof(No));
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
        if (**str == ')')
            (*str)++;
        else {
            fprintf(stderr, "Erro: Parenteses ')' nao fechados na funcao '%s'.\n", nomeFuncao);
            liberarArvore(argumento);
            exit(1);
        }
        No *noFuncao = safeMalloc(sizeof(No));
        noFuncao->tipo = 'F';
        strcpy(noFuncao->funcao, nomeFuncao);
        noFuncao->esquerda = argumento;
        noFuncao->direita = NULL;
        return noFuncao;
    } else {
        fprintf(stderr, "Erro: Esperado '(' apos a funcao '%s'.\n", nomeFuncao);
        exit(1);
    }
}

double avaliar(No *no, int *passo) {
    if (no->tipo == 'N')
        return no->valor;
    else if (no->tipo == 'F') {
        double arg = avaliar(no->esquerda, passo);
        double resultado = 0;
        if (strcmp(no->funcao, "sin") == 0)
            resultado = sin(arg);
        else if (strcmp(no->funcao, "cos") == 0)
            resultado = cos(arg);
        else if (strcmp(no->funcao, "tan") == 0)
            resultado = tan(arg);
        else if (strcmp(no->funcao, "ln") == 0) {
            if (arg <= 0) {
                fprintf(stderr, "Erro: Logaritmo de numero nao positivo.\n");
                exit(1);
            }
            resultado = log(arg);
        } else if (strcmp(no->funcao, "sqrt") == 0) {
            if (arg < 0) {
                fprintf(stderr, "Erro: Raiz quadrada de numero negativo.\n");
                exit(1);
            }
            resultado = sqrt(arg);
        } else {
            fprintf(stderr, "Erro: Funcao desconhecida '%s'.\n", no->funcao);
            exit(1);
        }
        printf("%d. %s(%.5f) = %.5f\n", *passo, no->funcao, arg, resultado);
        (*passo)++;
        no->tipo = 'N';
        no->valor = resultado;
        liberarArvore(no->esquerda);
        no->esquerda = no->direita = NULL;
        return resultado;
    } else if (no->tipo == 'O') {
        double esquerda = no->esquerda ? avaliar(no->esquerda, passo) : 0;
        double direita = no->direita ? avaliar(no->direita, passo) : 0;
        double resultado = 0;
        switch (no->operador) {
            case '+': resultado = esquerda + direita; break;
            case '-': resultado = esquerda - direita; break;
            case '*': resultado = esquerda * direita; break;
            case '/':
                if (direita == 0) {
                    fprintf(stderr, "Erro: Divisao por zero.\n");
                    exit(1);
                }
                resultado = esquerda / direita;
                break;
            case '^': resultado = pow(esquerda, direita); break;
            case '!':
                if (esquerda < 0 || floor(esquerda) != esquerda) {
                    fprintf(stderr, "Erro: Fatorial indefinido para numeros negativos ou nao inteiros.\n");
                    exit(1);
                }
                resultado = fatorial(esquerda);
                break;
            default:
                fprintf(stderr, "Erro: Operador desconhecido '%c'.\n", no->operador);
                exit(1);
        }
        printf("%d. ", *passo);
        (*passo)++;
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
    if (!no)
        return;
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
    } else if (no->tipo == 'N')
        printf("%.5f", no->valor);
}

double fatorial(double n) {
    if (n == 0 || n == 1)
        return 1;
    double resultado = 1;
    for (int i = 2; i <= (int)n; i++)
        resultado *= i;
    return resultado;
}

void liberarArvore(No *no) {
    if (!no)
        return;
    liberarArvore(no->esquerda);
    liberarArvore(no->direita);
    free(no);
}

int main() {
    setlocale(LC_ALL, "");
    printf("Bem-vindo a Calculadora!\n");
    printf("Operacoes: +, -, *, /, ^, sqrt, %%, !\n");
    printf("Funcoes: sin, cos, tan, ln, sqrt\n");
    printf("Constantes: pi, e\n");
    printf("Agrupamentos: (), [], {}\n");
    printf("Exemplo: 3+sin(45deg)*3-(32+[5+1]*2(3/5))^3-sqrt(9)+{3!}+(50%%*1000)\n\n");

    char expressao[TAM_MAX_EXPRESSAO];
    printf("Digite a expressao matematica:\n");
    if (!fgets(expressao, sizeof(expressao), stdin)) {
        fprintf(stderr, "Erro na leitura da expressao.\n");
        return 1;
    }
    size_t len = strlen(expressao);
    if (len > 0 && expressao[len - 1] == '\n')
        expressao[len - 1] = '\0';

    char *p = expressao;
    No *arvore = analisarExpressao(&p);
    pularEspacos(&p);
    if (*p != '\0') {
        fprintf(stderr, "Erro: Expressao invalida ou caracteres inesperados no final.\n");
        liberarArvore(arvore);
        return 1;
    }

    printf("\nPasso a Passo da Resolucao:\n");
    int passo = 1;
    double resultado = avaliar(arvore, &passo);
    printf("\nResultado Final: %.5f\n", resultado);
    liberarArvore(arvore);
    return 0;
}
