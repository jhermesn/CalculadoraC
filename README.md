# Calculadora em C

### Introdução
Bem-vindo à minha primeira calculadora (todo programador tem que ter uma, né?)! Esta calculadora suporta uma variedade de operações e funções matemáticas, tornando cálculos complexos simples e diretos.

### Funcionalidades
- **Operações:** Adição (`+`), Subtração (`-`), Multiplicação (`*`), Divisão (`/`), Exponenciação (`^`), Raiz Quadrada (`sqrt`), Porcentagem (`%`), Fatorial (`!`)
- **Funções:** Seno (`sin`), Cosseno (`cos`), Tangente (`tan`), Logaritmo Natural (`ln`), Raiz Quadrada (`sqrt`)
- **Constantes:** Pi (`pi`), Número de Euler (`e`)
- **Símbolos de Agrupamento:** Parênteses `()`, Colchetes `[]`, Chaves `{}`
- **Indicador de Grau:** Use `deg` após um número para indicar graus em funções trigonométricas (ex.: `sin(45deg)`)
- **Multiplicação Implícita:** Suporta multiplicação implícita (ex.: `2(3) = 2 * 3`)
- **Operador de Porcentagem:** O símbolo `%` é usado como operador de porcentagem (ex.: `50% = 0.5`)
- **Resolução Passo a Passo:** Exibe cada passo do processo de cálculo para melhor compreensão.

### Instalação
1. **Clone o Repositório:**
   ```bash
   git clone https://github.com/JorgeHermes/Calculadora.git
   ```
2. **Navegue até o Diretório do Projeto:**
   ```bash
   cd Calculadora
   ```
3. **Compile o Programa:**
   ```bash
   gcc calculadora.c -o calculadora -lm
   ```
   > **Nota:** A flag `-lm` vincula a biblioteca matemática.

### Uso
1. **Execute a Calculadora:**
   ```bash
   ./calculadora
   ```
2. **Siga as Instruções na Tela:**
    - Insira sua expressão matemática quando solicitado.
    - Exemplo:
      ```
      3+sin(45deg)*3-(32+[5+1]*2(3/5))^3-sqrt(9)+{3!}+(50%*1000)
      ```

### Contribuindo
Contribuições são bem-vindas! Por favor, faça um fork do repositório e submeta um pull request com suas melhorias.

### Licença
Este projeto está licenciado sob a [Licença MIT](LICENSE).

### Créditos
- **Criador:** [Jorge Hermes](https://jhermesn.dev/)
