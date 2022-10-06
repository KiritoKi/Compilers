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
char glc[100][20];        //[<linhas>][<armazenamento por linha>]
char lines_input[40][20]; //[<linhas>][<armazenamento por linha>]
// ReadFileFunctions
int readINPUT(FILE *file_IN); // Retorna quantidade de linhas de entradas
int readGLC(FILE *file_GLC);
//--> AUTOMATO <--
int automato(char *in, int qtd_linesGLC);
// PrintFunctions
void printGLC(int qtd_line);
void printTable();
void printTree();
// StackFunctions
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
        /*if (result == 1)
        //{
             table();
             Tree();
        }*/
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

int automato(char *in, int qtd_linesGLC)
{
    int i = 0; // line-glc control
    goto Q0;

Q0:
    // Se o programa correr a linguagem e não encontrar nenhum δ : δ(ε,ε,XXX)
    // Ou as tentativas existentes forem inválidas
    if (i > qtd_linesGLC)
        goto ERROR;
    // Se a pilha estiver zerada e existir uma δ : δ(ε,ε,XXX) na linguagem
    if (STACK_TOP == -1 && in[0] == '_' && in[1] == '_')
    {
        push(in[2]); // WARNING ---------> Ainda funciona apenas com 1 caractere de valor inicial
    }

    i++;
Q1:

ACCEPT:
    printf("\nAccept : %s ", in);
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
            // Adiciona na proxima coluna a condição de parada para ler
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
            // Adiciona na proxima coluna a condição de parada para ler
            lines_input[i][j] = '\0';
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
    printf("\n\n");
}

void printTable()
{
    printf("--> Table - Parsing <--\n");
    printf("%-15s %-15s %-15s %-15s %-15s %s\n", "i", "q", ".w", "Stack", "δ");
}

void printTree()
{
    printf("--> GLC - Gramatica Livre de Contexto <--\n");
}