#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACKSIZE 500

int STACK_TOP = -1;
char STACK[STACKSIZE];
char glc[40][20];         //[<linhas>][<armazenamento por linha>]
char lines_input[40][20]; //[<linhas>][<armazenamento por linha>]

int readINPUT(FILE *file_IN); // Retorna quantidade de linhas de entradas
void readGLC(FILE *file_GLC);
void push(char value);
void pop();
void error();
void Q0(int i);
void Q1(int i);

void main()
{
    FILE *file_GLC;
    FILE *file_IN;
    char *result;
    int qtd_entries;
    // Abre um arquivo TEXTO para LEITURA
    file_GLC = fopen("GLC-file.txt", "rt");
    file_IN = fopen("input_file.txt", "rt");
    if (file_GLC == NULL) // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return;
    }

    readGLC(file_GLC);
    qtd_entries = readINPUT(file_IN);

    for (int i = 0; i < qtd_entries; i++)
    {
        automato(lines_input[i]);
    }

    printf("%s", glc[0]);
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

void automato(char in[])
{
    prinft("%c", in[0]);
};
void Q0(int i)
{

    printf("");
    else
    {
        printf("O ");
    }
}

void readGLC(FILE *file_GLC)
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
        }
        // Se chegar no fim da linha armazena na proxima linha da matriz
        if (read_char == '\n')
        {
            i++;
            j = 0;
            continue;
        }
        // Atribui os caracteres no vetor que armazena o glc
        glc[i][j] = read_char;
        j++;
    }
    return glc;
}

int readINPUT(FILE *file_IN)
{
    char read_char;
    int i = 0, j = 0;

    while ((read_char = getc(file_IN)) != EOF)
    {
        printf("%c   %d\n", read_char, j);
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