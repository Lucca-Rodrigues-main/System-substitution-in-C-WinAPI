#define preto_           0
#define azul_            1
#define verde_           2
#define ciano_           3
#define vermelho_        4
#define magenta_         5
#define marrom_          6
#define cinza_claro_     7
#define cinza_escuro_    8
#define azul_claro_      9
#define verde_claro_    10
#define ciano_claro_    11
#define vermelho_claro_ 12
#define magenta_claro_  13
#define amarelo_        14
#define branco_         15

#include <stdio.h>
#include <windows.h>

// Substituto para system("cls") ou system("clear")
void LimpaTela(int texto, int fundo); // Limpa a tela da console

// Substitutos para system("pause")
int PausaKB(void); // Aguarda por uma tecla do teclado
int PausaEnter(void); // Aguarda pela tecla Enter

// Substitutos para system("color XY");
void CorTexto(int texto, int fundo); // Muda as cores do texto e do fundo
void RevCorTexto(void); // Inverte as cores do texto e do fundo
int PrintfCor(int texto, int fundo,
              const char* format, ...); // Imprime dados formatados com cores de texto e fundo alteradas
int PrintfRevCor(const char* format, ...); // Imprime dados formatados com cores de texto e fundo invertidas

int main()
{
    printf("Texto normal\n\n");

    RevCorTexto();
    printf("Cor invertida com RevCorTexto()\n\n");

    CorTexto(vermelho_claro_, ciano_);
    printf("Cor alterada com CorTexto()\n\n");

    CorTexto(cinza_claro_, preto_);
    printf("Cor restaurada com CorTexto()\n\n");

    PrintfCor(verde_, branco_, "Cor alterada (somente neste printf) com PrintfCor()\n");
    printf("Texto normal\n\n");

    PrintfRevCor("Cor invertida (somente neste printf) com PrintfCor()\n");
    printf("Texto normal\n\n");

    printf("Teste da PausaEnter()\nPressione Enter para continuar...\n");
    PausaEnter();

    printf("\nTeste da PausaKB()\nPressione qualquer tecla para continuar...\n");
    PausaKB();

    return 0;
}

void LimpaTela(int texto, int fundo) // Limpa a tela da console
{
    WORD wColor = ((fundo & 0x0F) << 4) + (texto & 0x0F);
    // Obter o handle do buffer de saída atual...
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //Isso é usado para levar o cursor ao canto superior esquerdo (posição <0,0>).
    COORD coord = {0, 0};
    /* Um valor de retorno... Indicando quantos caracteres foram escritos e não
    usados, mas precisamos disso, pois será escrito de qualquer forma (passar
    NULL causa um "access violation"). */
    DWORD cont;

    /* Esta é uma estrutura que contém todas as informações da console, e é
    usada aqui para encontrar o tamanho da console. */
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // Aqui vamos definir a cor atual
    SetConsoleTextAttribute(hStdOut, wColor);
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        // Isso preenche o buffer com um determinado caractere (neste caso, 32 = espaço).
        FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &cont);

        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &cont );
        // Isso deve definir a posição do cursor para a próxima impressão
        SetConsoleCursorPosition(hStdOut, coord);
    }
    return;
}

int PausaKB(void)
{
	INPUT_RECORD record;
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	int total = 0;

	do {
		FlushConsoleInputBuffer(console);
		WaitForSingleObject(console, INFINITE);

		/* Se PeekConsoleInput() for bem-sucedida,
	    o valor retornado será diferente de zero */
		if(!PeekConsoleInput(console, &record, 1, (LPDWORD)&total))
            return 1;
		if (total > 0)
            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
            {
                FlushConsoleInputBuffer(console);
                /* Se o evento de entrada for um
                evento do teclado, retorna 0 */
                return 0;
            }
	} while(1);
}

int PausaEnter(void)
{
	INPUT_RECORD record;
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	int total = 0;
	// Tecla
	WORD v = 0;

	do {
		FlushConsoleInputBuffer(console);
		WaitForSingleObject(console, INFINITE);

		/* Se PeekConsoleInput() for bem-sucedida,
	    o valor retornado será diferente de zero */
		if(!PeekConsoleInput(console, &record, 1, (LPDWORD)&total))
            return 1;
		if (total > 0)
            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
            {
                v = record.Event.KeyEvent.wVirtualKeyCode;
                FlushConsoleInputBuffer(console);
                /* Se o evento de entrada for um evento do
                teclado e o código de chave virtual tiver
                o valor da tecla Enter, retorna 0 */
                if (v == VK_RETURN) return 0;
            }
	} while(1);
}

void CorTexto(int texto, int fundo) // Muda as cores do texto e do fundo
{
    /* SetConsoleTextAttribute define os atributos de
    caracteres gravados no buffer da tela do console */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), texto | (fundo << 4));

	return;
}

void RevCorTexto(void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    // Obtendo informações sobre o buffer de tela do console
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	WORD texto = info.wAttributes & 0xF;
	WORD fundo = info.wAttributes & 0xF0;

    /* SetConsoleTextAttribute define os atributos de
    caracteres gravados no buffer da tela do console */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fundo | (texto << 4));

	return;
}

int PrintfCor(int texto, int fundo, const char* format, ...)
{
    int res;
    CONSOLE_SCREEN_BUFFER_INFO info;

    // Obtendo informações sobre o buffer de tela do console
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	WORD OldTexto = info.wAttributes & 0xF;
	WORD OldFundo = info.wAttributes & 0xF0;

    va_list args;
    va_start(args, format);

    // Alterando os atributos dos caracteres prestes a serem gravados
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), texto | (fundo << 4));

    res = vprintf(format, args);

    // Restaurando
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), OldTexto | (OldFundo << 4));

    va_end(args);

    return res;
}

int PrintfRevCor(const char* format, ...)
{
    int res;
    CONSOLE_SCREEN_BUFFER_INFO info;

    // Obtendo informações sobre o buffer de tela do console
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	WORD texto = info.wAttributes & 0xF;
	WORD fundo = info.wAttributes & 0xF0;

    va_list args;
    va_start(args, format);

    // Alterando os atributos dos caracteres prestes a serem gravados
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fundo | (texto << 4));

    res = vprintf(format, args);

    // Restaurando
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), texto | (fundo << 4));

    va_end(args);

    return res;
}
