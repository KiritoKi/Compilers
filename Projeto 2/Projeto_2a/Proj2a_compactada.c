#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Delimitador para ler a linha do arquivo
//(deve conter no final da cada linha até da ultima DEFAULT = enter)

//-------------------->>Primeira linha do GLC-file deve ser _,_,S<<------
//----------------------->>Favor usar '_' no lugar de 'ε'<<------------------------
//------>>ATENÇÃO NO DELIMITER DE LINHA, DEVE EXISTIR NO FINAL DA LINHA NOS ARQUIVOS<<------------------------


typedef struct hash_tree
{
    int id;
    char value;

} Htree;

#define TRUE 1
#define FALSE -1
#define STACKSIZE 500
#define DELIMITER '\n'

int STACK_TOP = -1;
char STACK[STACKSIZE];
char lines_input[40][150]; //[<linhas>][<armazenamento por linha>]
int STEP = -1;

int p_count = 0;
int productions[100] = {-1};

// ReadFileFunctions
int readINPUT(FILE *file_IN); // Retorna quantidade de linhas de entradas
//--------< AUTOMATO >--------
void automato(char *in);
//--------< TREE >--------
void parsing_tree();
// -------<PrintFunctions>-------
void printTableHead();
void printTreeHead();
void printLineTable(char *q, char *w, char *d, char *p, int token);
// -------<StackFunctions>--------
void inversePush(char *line);
void push(char value);
void pop();


int main()
{

    FILE *file_IN;
    int qtd_entries;
    // Abre um arquivo TEXTO para LEITURA
    file_IN = fopen("input_file.txt", "rt");
    if (file_IN == NULL) // Se houver erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return -1;
    }
    qtd_entries = readINPUT(file_IN);
    printf("lines to input: %d\n\n", qtd_entries);
    for (int i = 0; i < qtd_entries; i++)
    {
        p_count = 0;
        STEP = -1;
        printTableHead();
        automato(lines_input[i]);
        printTreeHead();
        parsing_tree();
    }

    fclose(file_IN);
}
// Nao ta sendo utilizado ainda <-A UNSUAL PUSH inverte os caracteres a linha antes de dar push---->
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
    lines_input[i][j] = '\0';
    i++;
    return i;
}

void printTableHead()
{
    printf("\t\t--> Table - Parsing <--\n");
    printf("%-5s %-18s %-30s %-20s %-5s %5s\n", "i", "q", ".w", "Stack", "δ", "p");
}

void printTreeHead()
{
    printf("\t\t--> Tree - Parsing <--\n");
}

void printLineTable(char *q, char *w, char *d, char *p, int token)
{
    STEP++;

    char stackPrint[STACKSIZE];
    int i = STACK_TOP;
    int j = 0;
    while (i > -1)
    {
        stackPrint[j] = STACK[i];
        i--;
        j++;
    }
    stackPrint[j] = '\0';

    char w1[STACKSIZE];
    for (i = 0; i < token; i++)
        w1[i] = w[i];
    w1[i] = '\0';

    char w2[STACKSIZE];
    for (j = 0; w[j] != '\0'; j++)
        w2[j] = w[token + j];
    w2[j] = '\0';

    printf("%-5d %-5s %s.%s %20s %-5s %5s\n", STEP, q, w1, w2, stackPrint, d, p);
    return;
}

void automato(char *in)
{
    int token = 0;
    char tree[100] = {'\0'}; // Foi obrigatorio o uso de vetor ao invés de struct dinamica
    int count_tree = 0;
    int visited[100] = {0};
    int aux_f[100] = {0};
    int aux_ind = 0;
    int aux;
    goto Q0;

Q0:
    tree[count_tree] = 'S';
    visited[count_tree] = TRUE;
    
    printLineTable("Q0", in, "δ0", "-", 0);
    push('S'); // stack[0] = 'S'
    goto Q1;

    // NODE Q1
Q1get:
    token += 1;

Q1:
    if (in[token] == '\0')
    {
        printLineTable("Q1", in, "-", "-", token);
        goto ACCEPT;
    }
    if ((in[token] == 'm') && (STACK[STACK_TOP] == 'S'))
    { // P1    S-> M   //t1
        productions[p_count++] = 1; 
        printLineTable("Q1", in, "δ1", "p1", token);
        pop();
        push('M');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'M';

        goto Q1;
    }
    else if ((in[token] == 'g') && (STACK[STACK_TOP] == 'S'))
    { // P2  S-> G M      //t2
        productions[p_count++] = 2; 
        printLineTable("Q1", in, "δ2", "p2", token);
        pop();
        push('M');
        push('G');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'G';
        // tree[++count_tree] = 'M';

        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'S'))
    { // P3   S -> F G M      //t3
        productions[p_count++] = 3; 
        printLineTable("Q1", in, "δ3", "p3", token);
        pop();
        push('M');
        push('G');
        push('F');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'F';
        // tree[++count_tree] = 'G';
        // tree[++count_tree] = 'M';

        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'F'))
    { // P4 - F -> f(){C;r(E);}      //t4
        productions[p_count++] = 4; 
        printLineTable("Q1", in, "δ4", "p4", token);
        pop();
        push('}');
        push(';');
        push(')');
        push('E');
        push('(');
        push('r');
        push(';');
        push('C');
        push('{');
        push(')');
        push('(');
        push('f');
        // count_tree = 12 * count_tree + 1;

        // tree[count_tree] = 'f';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = '{';
        // tree[++count_tree] = 'C';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = 'r';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = '}';
        goto Q1;
    }
    else if ((in[token] == 'g') && (STACK[STACK_TOP] == 'G'))
    { // P5 - G -> g(){C;r(E);}      //t5
        productions[p_count++] = 5; 
        printLineTable("Q1", in, "δ5", "p5", token);
        pop();
        push('}');
        push(';');
        push(')');
        push('E');
        push('(');
        push('r');
        push(';');
        push('C');
        push('{');
        push(')');
        push('(');
        push('g');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree++] = 'g';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = '{';
        // tree[++count_tree] = 'C';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = 'r';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = '}';
        goto Q1;
    }
    else if ((in[token] == 'm') && (STACK[STACK_TOP] == 'M'))
    { // P6 - M -> m(){C;r(E);}      //t6
        productions[p_count++] = 6; 
        printLineTable("Q1", in, "δ6", "p6", token);
        pop();
        push('}');
        push(';');
        push(')');
        push('E');
        push('(');
        push('r');
        push(';');
        push('C');
        push('{');
        push(')');
        push('(');
        push('m');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'm';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = '{';
        // tree[++count_tree] = 'C';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = 'r';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = '}';
        goto Q1;
    }
    else if ((in[token] == '0') && (STACK[STACK_TOP] == 'E'))
    { // P7 - E -> 0      //t7
        productions[p_count++] = 7; 
        printLineTable("Q1", in, "δ7", "p7", token);
        pop();
        push('0');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '0';
        goto Q1;
    }
    else if ((in[token] == '1') && (STACK[STACK_TOP] == 'E'))
    { // P8 - E -> 1      //t8
        productions[p_count++] = 8; 
        printLineTable("Q1", in, "δ8", "p8", token);
        pop();
        push('1');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '1';
        goto Q1;
    }
    else if ((in[token] == 'x') && (STACK[STACK_TOP] == 'E'))
    { // P9 - E -> x      //t9
        productions[p_count++] = 9; 
        printLineTable("Q1", in, "δ9", "p9", token);
        pop();
        push('x');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'x';
        goto Q1;
    }
    else if ((in[token] == 'y') && (STACK[STACK_TOP] == 'E'))
    { // P10 - E -> y      //t10
        productions[p_count++] = 10; 
        printLineTable("Q1", in, "δ10", "p10", token);
        pop();
        push('y');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'y';
        goto Q1;
    }
    else if ((in[token] == '(') && (STACK[STACK_TOP] == 'E'))
    { // P11 - E -> (EXE)      //t11
        productions[p_count++] = 11; 
        printLineTable("Q1", in, "δ11", "p11", token);
        pop();
        push(')');
        push('E');
        push('X');
        push('E');
        push('(');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = 'X';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';

        goto Q1;
    }
    else if ((in[token] == '+') && (STACK[STACK_TOP] == 'X'))
    { // P12 - X -> +      //t12
        productions[p_count++] = 12; 
        printLineTable("Q1", in, "δ12", "p12", token);
        pop();
        push('+');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '+';
        goto Q1;
    }
    else if ((in[token] == '-') && (STACK[STACK_TOP] == 'X'))
    { // P13 - X -> -      //t13
        productions[p_count++] = 13; 
        printLineTable("Q1", in, "δ13", "p13", token);
        pop();
        push('-');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '-';
        goto Q1;
    }
    else if ((in[token] == '*') && (STACK[STACK_TOP] == 'X'))
    { // P14 - X -> *      //t14
        productions[p_count++] = 14; 
        printLineTable("Q1", in, "δ14", "p14", token);
        pop();
        push('*');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '*';
        goto Q1;
    }
    else if ((in[token] == '/') && (STACK[STACK_TOP] == 'X'))
    { // P15 - X -> /      //t15
        productions[p_count++] = 15; 
        printLineTable("Q1", in, "δ15", "p15", token);
        pop();
        push('/');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '/';
        goto Q1;
    }
    else if ((in[token] == 'h') && (STACK[STACK_TOP] == 'C'))
    { // P16 - C -> h=E      //t16
        productions[p_count++] = 16; 
        printLineTable("Q1", in, "δ16", "p16", token);
        pop();
        push('E');
        push('=');
        push('h');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'h';
        // tree[++count_tree] = '=';
        // tree[++count_tree] = 'E';
        goto Q1;
    }
    else if ((in[token] == 'i') && (STACK[STACK_TOP] == 'C'))
    { // P17 - C -> i=E      //t17
        productions[p_count++] = 17; 
        printLineTable("Q1", in, "δ17", "p17", token);
        pop();
        push('E');
        push('=');
        push('i');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'i';
        // tree[++count_tree] = '=';
        // tree[++count_tree] = 'E';
        goto Q1;
    }
    else if ((in[token] == 'j') && (STACK[STACK_TOP] == 'C'))
    { // P18 - C -> j=E      //t18
        productions[p_count++] = 18; 
        printLineTable("Q1", in, "δ18", "p18", token);
        pop();
        push('E');
        push('=');
        push('j');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'j';
        // tree[++count_tree] = '=';
        // tree[++count_tree] = 'E';
        goto Q1;
    }
    else if ((in[token] == 'k') && (STACK[STACK_TOP] == 'C'))
    { // P19 - C -> k=E      //t19
        productions[p_count++] = 19; 
        printLineTable("Q1", in, "δ19", "p19", token);
        pop();
        push('E');
        push('=');
        push('k');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'k';
        // tree[++count_tree] = '=';
        // tree[++count_tree] = 'E';
        goto Q1;
    }
    else if ((in[token] == 'z') && (STACK[STACK_TOP] == 'C'))
    { // P20 - C -> z=E      //t20
        productions[p_count++] = 20; 
        printLineTable("Q1", in, "δ20", "p20", token);
        pop();
        push('E');
        push('=');
        push('z');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'z';
        // tree[++count_tree] = '=';
        // tree[++count_tree] = 'E';
        goto Q1;
    }
    else if ((in[token] == '(') && (STACK[STACK_TOP] == 'C'))
    { // P21 - C -> (EXE)      //t21
        productions[p_count++] = 21; 
        printLineTable("Q1", in, "δ21", "p21", token);
        pop();
        push(')');
        push('E');
        push('X');
        push('E');
        push('(');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = '(' ;
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = 'X';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        goto Q1;
    }
    else if ((in[token] == 'w') && (STACK[STACK_TOP] == 'C'))
    { // P22 - C -> w(E){C;}      //t22
        productions[p_count++] = 22; 
        printLineTable("Q1", in, "δ22", "p22", token);
        pop();
        push('}');
        push(';');
        push('C');
        push('{');
        push(')');
        push('E');
        push('(');
        push('w');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'w';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = '{';
        // tree[++count_tree] = 'C';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = '}';
        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'C'))
    { // P23 - C -> f(E){C;}      //t23
        productions[p_count++] = 23; 
        printLineTable("Q1", in, "δ23", "p23", token);
        pop();
        push('}');
        push(';');
        push('C');
        push('{');
        push(')');
        push('E');
        push('(');
        push('f');
        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'f';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = '{';
        // tree[++count_tree] = 'C';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = '}';
        goto Q1;
    }
    else if ((in[token] == 'o') && (STACK[STACK_TOP] == 'C'))
    { // P24 - C -> o(E;E;E){C;}      //t24
        productions[p_count++] = 24; 
        printLineTable("Q1", in, "δ24", "p24", token);
        pop();
        push('}');
        push(';');
        push('C');
        push('{');
        push(')');
        push('E');
        push(';');
        push('E');
        push(';');
        push('E');
        push('(');
        push('o');

        // count_tree = 12 * count_tree + 1;
        // tree[count_tree] = 'o';
        // tree[++count_tree] = '(';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = 'E';
        // tree[++count_tree] = ')';
        // tree[++count_tree] = '{';
        // tree[++count_tree] = 'C';
        // tree[++count_tree] = ';';
        // tree[++count_tree] = '}';
        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'f'))
    { // token_accepted = f      //t25
        printLineTable("Q1", in, "δ25", "-", token);
        pop();

        goto Q1get;
    }
    else if ((in[token] == 'g') && (STACK[STACK_TOP] == 'g'))
    { // token_accepted = g     //t26
        printLineTable("Q1", in, "δ26", "-", token);
        pop();

        goto Q1get;
    }
    else if ((in[token] == 'm') && (STACK[STACK_TOP] == 'm'))
    { // token_accepted = m     //t27
        printLineTable("Q1", in, "δ27", "-", token);
        pop();

        goto Q1get;
    }
    else if ((in[token] == '(') && (STACK[STACK_TOP] == '('))
    { // token_accepted = (     //t28
        printLineTable("Q1", in, "δ28", "-", token);
        pop();

        goto Q1get;
    }
    else if ((in[token] == ')') && (STACK[STACK_TOP] == ')'))
    { // token_accepted = )     //t29
        printLineTable("Q1", in, "δ29", "-", token);
        pop();

        goto Q1get;
    }
    else if ((in[token] == '{') && (STACK[STACK_TOP] == '{'))
    { // token_accepted = {     //t30
        printLineTable("Q1", in, "δ30", "-", token);
        pop();

        goto Q1get;
    }
    else if ((in[token] == '}') && (STACK[STACK_TOP] == '}'))
    { // token_accepted = }     //t31
        printLineTable("Q1", in, "δ31", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'r') && (STACK[STACK_TOP] == 'r'))
    { // token_accepted = r     //t32
        printLineTable("Q1", in, "δ32", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == ';') && (STACK[STACK_TOP] == ';'))
    { // token_accepted = ;     //t33
        printLineTable("Q1", in, "δ33", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '0') && (STACK[STACK_TOP] == '0'))
    { // token_accepted = 0     //t34
        printLineTable("Q1", in, "δ34", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '1') && (STACK[STACK_TOP] == '1'))
    { // token_accepted = 1     //t35
        printLineTable("Q1", in, "δ35", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'x') && (STACK[STACK_TOP] == 'x'))
    { // token_accepted = x     //t36
        printLineTable("Q1", in, "δ36", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'y') && (STACK[STACK_TOP] == 'y'))
    { // token_accepted = y     //t37
        printLineTable("Q1", in, "δ37", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '+') && (STACK[STACK_TOP] == '+'))
    { // token_accepted = +     //t38
        printLineTable("Q1", in, "δ38", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '-') && (STACK[STACK_TOP] == '-'))
    { // token_accepted = -     //t39
        printLineTable("Q1", in, "δ39", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '*') && (STACK[STACK_TOP] == '*'))
    { // token_accepted = *     //t40
        printLineTable("Q1", in, "δ40", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '/') && (STACK[STACK_TOP] == '/'))
    { // token_accepted = /     //t41
        printLineTable("Q1", in, "δ41", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'h') && (STACK[STACK_TOP] == 'h'))
    { // token_accepted = h     //t42
        printLineTable("Q1", in, "δ42", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'i') && (STACK[STACK_TOP] == 'i'))
    { // token_accepted = i     //t43
        printLineTable("Q1", in, "δ43", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'j') && (STACK[STACK_TOP] == 'j'))
    { // token_accepted = j     //t44
        printLineTable("Q1", in, "δ44", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'k') && (STACK[STACK_TOP] == 'k'))
    { // token_accepted = k     //t45
        printLineTable("Q1", in, "δ45", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'z') && (STACK[STACK_TOP] == 'z'))
    { // token_accepted = z     //t46
        printLineTable("Q1", in, "δ46", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == '=') && (STACK[STACK_TOP] == '='))
    { // token_accepted = =     //t47
        printLineTable("Q1", in, "δ47", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'w') && (STACK[STACK_TOP] == 'w'))
    { // token_accepted = w     //t48
        printLineTable("Q1", in, "δ48", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'f'))
    { // token_accepted = f      //t49
        printLineTable("Q1", in, "δ49", "-", token);
        pop();
        goto Q1get;
    }
    else if ((in[token] == 'o') && (STACK[STACK_TOP] == 'o'))
    { // token_accepted = o     //t50
        printLineTable("Q1", in, "δ50", "-", token);
        pop();
        goto Q1get;
    }
    else if (in[token] == ' ')
    {
        goto Q1get;
    }
    else
    {
        goto ERROR;
    }
ACCEPT:
    printf("\nAccepted : %s\n", in);
    printf("-----------------------------------------------------------------------------------------------\n");
    return;

ERROR:
    printf("\nRejected : %s\n", in);
    printf("-----------------------------------------------------------------------------------------------\n");
    return;
}

void parsing_tree(){
    int max = 8096;
    char tree[max];
    int visited[max];
    int i=0, top=0;
    int tam_max_deriv = 4;
    
TREE_Q0:
    
    for (int j = 0; j < max; j++) { 
        tree[j] = ' '; 
        visited[j] = FALSE; 
    }
    tree[0] = 'S';
    goto TREE_Q1;
TREE_Q1:
        
    if(i>=p_count)
        goto TREE_FIM;
    // P1: S -> M
    if(productions[i] == 1){
        if (tree[top] == 'S' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'M'; 
            tree[tam_max_deriv*top + 2] = ' '; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+2] = TRUE; 
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P2: S -> GM
    else if(productions[i] == 2) {
        
        if (tree[top] == 'S' && tree[tam_max_deriv*top+1] == ' '){
            
            tree[tam_max_deriv*top + 1] = 'G'; 
            tree[tam_max_deriv*top + 2] = 'M'; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P3: S -> NGM
    else if(productions[i] == 3) {
        if (tree[top] == 'S' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'N'; 
            tree[tam_max_deriv*top + 2] = 'G'; 
            tree[tam_max_deriv*top + 3] = 'M'; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P4: N -> n(){ C; r(E); }
    else if(productions[i] == 4) {
        if (tree[top] == 'N' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'C'; 
            tree[tam_max_deriv*top + 2] = 'E'; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P5: G -> g(){ C; r(E); }
    else if(productions[i] == 5) {
        if (tree[top] == 'G' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'C'; 
            tree[tam_max_deriv*top + 2] = 'E'; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P6: M -> m() { C; r(E); }
    else if(productions[i] == 6) {
        if (tree[top] == 'M' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'C'; 
            tree[tam_max_deriv*top + 2] = 'E'; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+3] = TRUE;
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[i] >= 7 && productions[i] <= 10) {
        if (tree[top] == 'E' && tree[tam_max_deriv*top+1] == ' '){
            if (productions[i] == 7) 
                tree[tam_max_deriv*top + 1] = '0';
            else if (productions[i] == 8) 
                 tree[tam_max_deriv*top + 1] = '1'; 
            else if (productions[i] == 9) 
                tree[tam_max_deriv*top + 1] = 'x'; 
            else 
                tree[tam_max_deriv*top + 1] = 'y'; 
            tree[tam_max_deriv*top + 2] = ' '; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+1] = TRUE; 
            visited[tam_max_deriv*top+2] = TRUE; 
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P11: E -> (EXE)
    else if(productions[i] == 11) {
        if (tree[top] == 'E' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'E'; 
            tree[tam_max_deriv*top + 2] = 'X'; 
            tree[tam_max_deriv*top + 3] = 'E'; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[i] >= 12 && productions[i] <= 15) {
        if (tree[top] == 'X' && tree[tam_max_deriv*top+1] == ' '){
            if(productions[i] == 12) 
                tree[tam_max_deriv*top + 1] = '+'; 
            else if(productions[i] == 13) 
                tree[tam_max_deriv*top + 1] = '-'; 
            else if(productions[i] == 14) 
                tree[tam_max_deriv*top + 1] = '*'; 
            else 
                tree[tam_max_deriv*top + 1] = '/'; 
            tree[tam_max_deriv*top + 2] = ' '; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+1] = TRUE; 
            visited[tam_max_deriv*top+2] = TRUE; 
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P16: C -> h=E
    // P17: C -> i=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[i] >= 16 && productions[i] <= 20) {
        if (tree[top] == 'C' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'E'; 
            tree[tam_max_deriv*top + 2] = ' '; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+2] = TRUE; 
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P21: C -> (EXE)
    else if(productions[i] == 21) {
        if (tree[top] == 'C' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'E'; 
            tree[tam_max_deriv*top + 2] = 'X'; 
            tree[tam_max_deriv*top + 3] = 'E'; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P22: C -> w(E){ C; }
    else if(productions[i] == 22) {
        if (tree[top] == 'C' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'E'; 
            tree[tam_max_deriv*top + 2] = 'C'; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P23: C -> f(E){ C; }
    else if(productions[i] == 23) {
        if (tree[top] == 'C' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'E'; 
            tree[tam_max_deriv*top + 2] = 'C'; 
            tree[tam_max_deriv*top + 3] = ' '; 
            tree[tam_max_deriv*top + 4] = ' ';
            
            visited[tam_max_deriv*top+3] = TRUE; 
            visited[tam_max_deriv*top+4] = TRUE;
            i++;
        }
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[i] == 24) {
        if (tree[top] == 'C' && tree[tam_max_deriv*top+1] == ' '){
            tree[tam_max_deriv*top + 1] = 'E'; 
            tree[tam_max_deriv*top + 2] = 'E'; 
            tree[tam_max_deriv*top + 3] = 'E'; 
            tree[tam_max_deriv*top + 4] = 'C';
            i++;
        }
    }
    
    //Se encontrar um nó filho não visitado, visita ele e volta pro tree_q1
    if (visited[tam_max_deriv*top+1] == FALSE){ top = tam_max_deriv*top+1; goto TREE_Q1; }
    if (visited[tam_max_deriv*top+2] == FALSE){ top = tam_max_deriv*top+2; goto TREE_Q1; }
    if (visited[tam_max_deriv*top+3] == FALSE){ top = tam_max_deriv*top+3; goto TREE_Q1; }
    if (visited[tam_max_deriv*top+4] == FALSE){ top = tam_max_deriv*top+4; goto TREE_Q1; }

    int aux= 0;
    //Verifica se todas as derivações ja foram visitadas
    for (int i = 1; i<=tam_max_deriv; i++)
        if(visited[tam_max_deriv*top+i] == TRUE)
            aux++;
    //Se todas derivações foram visitadas, o pai tambem foi visitado
    if(aux == tam_max_deriv)
        visited[top] = TRUE;
    //Se o pai foi verificado, volta para o nó inicial e executa de novo o tree_q1
    if (visited[top] == TRUE) 
        top = 0; 
    
    goto TREE_Q1;

TREE_FIM:
    for (int i = 0; i< max; i++){
        if (tree[i] != ' ') { printf("%d, %c\n", i, tree[i]); }
    }

    printf("\n");
}