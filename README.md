# System-substitution-in-C-WinAPI
A set of functions that provide secure C alternatives to system() calls for console operations like clearing screen, pausing execution, and changing colors using Windows API.

---

## Introdução

### Por que evitar `system()`?
O uso da função `system()` é desencorajado por vários motivos importantes:

1. **Riscos de segurança**: `system()` pode ser explorada para injeção de comandos maliciosos;
2. **Proibições corporativas**: Muitas empresas e instituições de ensino banem seu uso;
3. **Inflexibilidade**: Não permite controle fino sobre o comportamento do programa;
4. **Dependência de plataforma**: Comandos podem variar entre sistemas operacionais;
5. **Performance**: Criar um shell externo é mais lento que chamadas nativas.

---

### Soluções Implementadas
Este projeto oferece alternativas seguras e eficientes para substituir as chamadas comuns a `system()`:

#### `LimpaTela(int texto, int fundo)`
Substitui `system("cls")`/`system("clear")` limpando a tela de forma programática usando a API do Windows. Aceita parâmetros para definir cores de texto e fundo.

#### `PausaKB(void)` e `PausaEnter(void)`
Substituem `system("pause")` com implementações mais seguras:
- `PausaEnter()` espera especificamente pela tecla Enter, sem de fato ler o que está na stdin. A implementação é baseada nas funções `WaitForSingleObject()`, que apenas aguarda por um evento no buffer de entrada, e `PeekConsoleInput`, que apenas "espia" o que há na stdin para poder validar a tecla Enter.
- `PausaKB()` espera por qualquer tecla. A implementação é a mesma da `PausaEnter()`, mas com uma maneira diferente de validar, sendo válido qualquer evento do teclado.

#### `CorTexto(int texto, int fundo)`
Substitui `system("color XY")` alterando cores do console de forma programática usando a API do Windows.

#### `RevCorTexto(void)`
Inverte as cores atuais do texto e fundo sem precisar de comandos de sistema.

#### `PrintfCor()` e `PrintfRevCor()`
Versões coloridas de `printf()` que:
- `PrintfCor()`: Imprime com cores específicas de maneira temporária, sem alterar os atributos de cores do console em si, ou seja, sem interferir nas próximas chamadas de `printf()` ou funções similares.
- `PrintfRevCor()`: Imprime com cores invertidas, também de maneira temporária.

---

### Exemplo de uso
O exemplo a seguir segue a implementação encontrada nesse repositório.

<img width="636" height="444" alt="image" src="https://github.com/user-attachments/assets/efc4f69a-6bba-4d4a-bcca-d0edc7c9d1d6" />
