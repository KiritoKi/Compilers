#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Delimitador para ler a linha do arquivo
//(deve conter no final da cada linha até da ultima DEFAULT = enter)

//----------------------->>Favor usar '_' no lugar de 'ε'<<------------------------
//------>>ATENÇÃO NO DELIMITER DE LINHA, DEVE EXISTIR NO FINAL DA LINHA NOS ARQUIVOS<<------------------------

#define STACKSIZE 500
#define DELIMITER '\n'

int STACK_TOP = -1;
char STACK[STACKSIZE];
char glc[40][20];         //[<linhas>][<armazenamento por linha>]
char lines_input[40][20]; //[<linhas>][<armazenamento por linha>]

int readINPUT(FILE *file_IN); // Retorna quantidade de linhas de entradas
int readGLC(FILE *file_GLC);
void printGLC(int qtd_line);
int automato(char *in);
void push(char value);
void pop();

int main()
{
    FILE *file_GLC;
    FILE *file_IN;
    // char *result;
    int qtd_entries;
    int qtd_lineGLC;
    int result; // Armazena 1 se aceitou e 0 se Rejeitou
    // Abre um arquivo TEXTO para LEITURA
    file_GLC = fopen("GLC-file.txt", "rt");
    file_IN = fopen("input_file.txt", "rt");
    if (file_GLC == NULL) // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return;
    }

    qtd_lineGLC = readGLC(file_GLC);
    qtd_entries = readINPUT(file_IN);

    printGLC(qtd_lineGLC);

    for (int i = 0; i < qtd_entries; i++)
    {
        result = automato(lines_input[i]);

        // Se o automato for aceito retorna a tabela e arvore
        if (result == 1)
        {
            // table();
            // Tree();
        }
    }

    fclose(file_GLC);
}

void push(char value)
{
    STACK_TOP++;
    STACK[STACK_TOP] = value;
}

void pop()
{
    STACK[STACK_TOP] = '\0';
    STACK_TOP--;
}

int automato(char *in)
{
Q0:
    if (STACK_TOP == -1)
        printf(".");
    else
        goto ERROR;
Q1:

ACCEPT:
    printf("\n\nAccept : %s ", in);
    return 1;
ERROR:
    printf("Reject : %s ", in);
    return 0;
};

int readGLC(FILE *file_GLC)
{
    int virgulini1 = 0;
    int virgulini2 = 0;
    char read_char;
    int i = 0, j = 0;

    while ((read_char = getc(file_GLC)) != EOF)
    {
        // Se a virgula não tiver na segunda e na quarta posição ele retorna erro
        // Só deve existir um caractere antes da primeira e antes da segunda virgula
        if (read_char == ',' && j == 1 || read_char == ',' && j == 2)
        {
            if (virgulini1 == 0)
            {
                virgulini1 = 1;
                continue;
            }
            else if (virgulini2 == 0)
                virgulini2 = 1;
            continue;
        }
        else if (read_char == ',' && j == 1 && virgulini1 == 0 || read_char == ',' && j == 2 && virgulini2 == 0)
        {
            printf("O Segundo e Quarto caractere da linha deve ser uma virgula! Verifique o input file!\n Ex: _,_,____\n");
            return -1;
        } /////>>-----------------------------------------------------<<
        // Se chegar no fim da linha armazena na proxima linha da matriz
        if (read_char == DELIMITER)
        {
            glc[i][j] = '\0';
            i++;
            j = 0;
            continue;
        }
        // Atribui os caracteres no vetor que armazena o glc
        glc[i][j] = read_char;
        j++;
    }
    return i + 1;
}

int readINPUT(FILE *file_IN)
{
    char read_char;
    int i = 0, j = 0;

    while ((read_char = getc(file_IN)) != EOF)
    {
        // Se chegar no fim da linha armazena na proxima linha da matriz
        if (read_char == '\n')
        {
            i++;
            j = 0;
            continue;
        }
        // Atribui os caracteres no vetor que armazena o input
        lines_input[i][j] = read_char;
        j++;
    }
    return i + 1;
}

void printGLC(int qtd_line)
{
    printf("--> GLC - Gramatica Livre de Contexto <--\n");

    char x;
    int j;
    for (int i = 0; i < qtd_line; i++)
    {
        j = 0;
        printf("\nδ : δ(%c, %c, ", glc[i][j], glc[i][j + 1]);
        j = 2;
        while (glc[i][j] != '\0')
        {
            printf("%c", glc[i][j]);
            j++;
        }
    }
}