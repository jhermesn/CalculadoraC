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
- **Criador:** [Jorge Hermes](https://jorgehermes.me/)

---

# Calculator in C

### Introduction
Welcome to my first calculator (every programmer should have one, right?)! This calculator supports a variety of mathematical operations and functions, making complex calculations simple and straightforward.

### Features
- **Operations:** Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`), Exponentiation (`^`), Square Root (`sqrt`), Percentage (`%`), Factorial (`!`)
- **Functions:** Sine (`sin`), Cosine (`cos`), Tangent (`tan`), Natural Logarithm (`ln`), Square Root (`sqrt`)
- **Constants:** Pi (`pi`), Euler's Number (`e`)
- **Grouping Symbols:** Parentheses `()`, Brackets `[]`, Braces `{}`
- **Degree Indicator:** Use `deg` after a number to indicate degrees in trigonometric functions (e.g., `sin(45deg)`)
- **Implicit Multiplication:** Supports implicit multiplication (e.g., `2(3) = 2 * 3`)
- **Percentage Operator:** The `%` symbol is used as a percentage operator (e.g., `50% = 0.5`)
- **Step-by-Step Resolution:** Displays each step of the calculation process for better understanding.

### Installation
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/JorgeHermes/Calculadora.git
   ```
2. **Navigate to the Project Directory:**
   ```bash
   cd Calculadora
   ```
3. **Compile the Program:**
   ```bash
   gcc calculadora.c -o calculadora -lm
   ```
   > **Note:** The `-lm` flag links the math library.

### Usage
1. **Run the Calculator:**
   ```bash
   ./calculadora
   ```
2. **Follow the On-Screen Instructions:**
    - Enter your mathematical expression when prompted.
    - Example:
      ```
      3+sin(45deg)*3-(32+[5+1]*2(3/5))^3-sqrt(9)+{3!}+(50%*1000)
      ```

### Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your improvements.

### License
This project is licensed under the [MIT License](LICENSE).

### Credits
- **Creator:** [Jorge Hermes](https://jorgehermes.me/)

---