#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Delimitador para ler a linha do arquivo
//(deve conter no final da cada linha até da ultima DEFAULT = enter)

//-------------------->>Primeira linha do GLC-file deve ser _,_,S<<------
//----------------------->>Favor usar '_' no lugar de 'ε'<<------------------------
//------>>ATENÇÃO NO DELIMITER DE LINHA, DEVE EXISTIR NO FINAL DA LINHA NOS ARQUIVOS<<------------------------

#define STACKSIZE 500
#define DELIMITER '\n'

int STACK_TOP = -1;
char STACK[STACKSIZE];
char glc[100][20];        //[<linhas>][<armazenamento por linha>]
char lines_input[40][20]; //[<linhas>][<armazenamento por linha>]
// Recovery Pilha
int level_sto[200] = {-1};
char trash_line_accepted[200][20];
int itera_trash = 0;

char *correct_string;

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
void inversePush(char *line);
void push(char value);
void pop();
// Recovery functions
void sto_line_accepted(char *in);
void lod_line_accepted();

void sto_line_accepted(char *in)
{
    int i = 0;
    // Insere a linha glc aceita no trash_line_accepted
    while (in[i] != '\0')
        trash_line_accepted[itera_trash][i] = in[i];
    // Incrementa o contador do armazenador de line_Accepted
    itera_trash++;
    return;
}
void lod_line_accepted()
{
    return;
}

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
        return -1;
    }

    qtd_lineGLC = readGLC(file_GLC);
    qtd_entries = readINPUT(file_IN);
    printf("%d  %d", qtd_lineGLC, qtd_entries);
    printGLC(qtd_lineGLC);
    for (int i = 0; i < qtd_entries; i++)
    {

        result = automato(lines_input[i], qtd_lineGLC);

        // Se o automato for aceito retorna a tabela e arvore
        /*if (result == 1)
        //{
             table();
             Tree();
        }*/
    }
    fclose(file_IN);
    fclose(file_GLC);
}

// A UNSUAL PUSH inverte os caracteres a linha antes de dar push---->
void inversePush(char *line)
{
    int i = 0;
    // Conta quantos caracteres tem na linha pra inserir no STACK
    while (line[i] != '\0')
    {
        i++;
    }
    i--;
    // Vai inserir inverso no STACK os caracteres exceto os 2 primeiros _ _ XXXXXXX
    while (i < 1)
    {
        push(line[i]);
        i--;
    }
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
    int itera_GLC = 0; // line-glc control
    int token = 0;     // char-input control
    int lvl_i = 0;     // level_sto control
    printf("aa ");
    // NODE Q0
Q0:
    // Se o programa correr a linguagem e não encontrar nenhum δ : δ(ε,ε,XXX)
    // Ou as tentativas existentes forem inválidas
    if (itera_GLC > qtd_linesGLC)
        goto ERROR;
    // Se a pilha estiver zerada e existir uma δ : δ(ε,ε,XXX) na linguagem
    // Ou seja a primeira linha do GLC-file deve ser _,_,S
    if (STACK_TOP == -1 && glc[itera_GLC][0] == '_' && glc[itera_GLC][1] == '_')
    {

        // sto_line_accepted(glc[itera_GLC]); // Apenas armazena a linha do glc que foi aceita(Usado para recuperar dado caso a expansão seja ambigua)

        inversePush(glc[itera_GLC]); // passa a linha do glc correspondente para um PUSH na pilha 'STACK'.
        itera_GLC = 0;
        goto Q1;
    }
    itera_GLC++;

    // NODE Q1
Q1:
    // Se o programa correr a linguagem e não encontrar nenhum parametro correspondente ao topo da pilha
    // Ou as tentativas existentes forem inválidas --> goto error
    if (itera_GLC > qtd_linesGLC)
    {
        // Se existir mais caminhos para testar ele volta 1 instancia e le as linhas de onde parou
        if (lvl_i != 0)
        {
            lvl_i--;
            token--;
            // lod_line_accepted();
            itera_GLC = level_sto[lvl_i];
            goto Q1;
        }
        goto ERROR;
    }

    // Se o programa terminar de ler a linha com sucesso e chegar ao fim -> goto accept
    if (in[token] == '\0')
        goto ACCEPT;

    // SE token atual é topo da pilha//SE token = 1º posição de _, _, ___ //Se token = 2º posição de _,_,___
    // VALIDAÇÃO DO CARACTERE INPUT E ENTÃO POP(STACK)
    if (in[token] == STACK[STACK_TOP] && in[token] == glc[itera_GLC][0] && in[token] == glc[itera_GLC][1])
    {
        // sto_line_accepted(glc[itera_GLC]); // Apenas armazena a linha do glc que foi aceita(Usado para recuperar dado caso a expansão seja ambigua)
        lvl_i++;

        pop();

        token++;
    }
    // Se o topo da pilha for o valor
    else if (glc[itera_GLC][1] == STACK[STACK_TOP])
    {
        // sto_line_accepted(glc[itera_GLC]); // Apenas armazena a linha do glc que foi aceita(Usado para recuperar dado caso a expansão seja ambigua)
        lvl_i++;

        pop();
        inversePush(glc[itera_GLC]);
        itera_GLC = 0;
        goto Q1;
    }

    itera_GLC++;
    goto Q1;
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
        // Se a virgula não tiver na segunda e na quarta posição dnas linha do GLC_FILE ele retorna erro
        // Só deve existir um caractere antes da primeira e antes da segunda virgula
        if ((read_char == ',' && j == 1) || (read_char == ',' && j == 2))
        {
            if (virgulini1 == 0)
                virgulini1 = 1;
            else if (virgulini2 == 0)
                virgulini2 = 1;
            continue;
        }
        else if ((read_char != ',' && j == 1 && virgulini1 == 0) || (read_char != ',' && j == 2 && virgulini2 == 0))
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
    int k = 0;
    // Se o delimitador for \n entao vai ter uma linha a mais no final do arq
    if (DELIMITER == '\n')
    {
        k = -1;
    }
    return i + k;
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
    int k = 0;
    // Se o delimitador for \n entao vai ter uma linha a mais no final do arq
    if (DELIMITER == '\n')
    {
        k = -1;
    }
    return i + k;
}

void printGLC(int qtd_line)
{
    printf("--> GLC - Gramatica Livre de Contexto <--\n");

    // char x;
    int j;
    for (int i = 0; i < qtd_line - 1; i++)
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
    // printf("%-15c %-15c %-15c %-15c %-15s %c\n", "i", "q", ".w", "Stack", "δ");
}

void printTree()
{
    printf("--> Tree -  <--\n");
}