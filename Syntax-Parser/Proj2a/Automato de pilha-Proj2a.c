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
#define true 1
#define false 0
#define bool int

int STACK_TOP = -1;
char STACK[STACKSIZE];
char lines_input[40][150]; //[<linhas>][<armazenamento por linha>]
int STEP = -1;
// ReadFileFunctions
int readINPUT(FILE *file_IN); // Retorna quantidade de linhas de entradas
//--> AUTOMATO <--
void automato(char *in);
// PrintFunctions
void printTableHead();
void printLineTable(char *q, char *w, char *d, char *p, int token);
void printTree();
// StackFunctions
void inversePush(char *line);
void push(char value);
void pop();

typedef struct node
{
    char key;
    struct node *firstSon;
    struct node *nextBrother;
} NODE;

typedef NODE *PONT;

PONT createNewNode(char key)
{
    PONT new = (PONT)malloc(sizeof(NODE));
    new->firstSon = NULL;
    new->nextBrother = NULL;
    new->key = key;
    return (new);
}

PONT inicializaTree(char key)
{
    return (createNewNode(key));
}

bool insere(PONT root, char newKey, char fatherKey)
{
    PONT father = searchKey(fatherKey, root);
    if (!father)
        return false;
    PONT son = createNewNode(newKey);
    PONT p = father->firstSon;
    if (!p)
        father->firstSon = son;
    else
    {
        while (p->nextBrother)
            p = p->nextBrother;
        p->nextBrother = son;
    }
    return (true);
}

void showTree(PONT root)
{
    if (root == NULL)
        return;
    printf("%c(", root->key);
    PONT p = root->firstSon;
    while (p)
    {
        showTree(p);
        p = p->nextBrother;
    }
    printf(")");
}

PONT searchKey(char key, PONT root)
{
    if (root == NULL)
        return NULL;
    if (root->key == key)
        return root;
    PONT p = root->firstSon;
    while (p)
    {
        PONT resp = searchKey(key, p);
        if (resp)
            return resp;
        p = p->nextBrother;
    }
    return (NULL);
}

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
        printTableHead();
        automato(lines_input[i]);
    }

    fclose(file_IN);
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

void printTree()
{
    printf("\n\n--> Tree - Parsing <--\n");
}

void automato(char *in)
{
    STEP = 0;
    int token = 0;
    char tree[sizeof(in)];

    goto Q0;

Q0:
    PONT tree = inicializaTree('S');
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
        printLineTable("Q1", in, "δ1", "p1", token);
        pop();
        push('M');
        goto Q1;
    }
    else if ((in[token] == 'g') && (STACK[STACK_TOP] == 'S'))
    { // P2  S-> G M      //t2
        printLineTable("Q1", in, "δ2", "p2", token);
        pop();
        push('M');
        push('G');
        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'S'))
    { // P3   S -> F G M      //t3
        printLineTable("Q1", in, "δ3", "p3", token);
        pop();
        push('M');
        push('G');
        push('F');
        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'F'))
    { // P4 - F -> f(){C;r(E);}      //t4
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
        goto Q1;
    }
    else if ((in[token] == 'g') && (STACK[STACK_TOP] == 'G'))
    { // P5 - G -> g(){C;r(E);}      //t5
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
        goto Q1;
    }
    else if ((in[token] == 'm') && (STACK[STACK_TOP] == 'M'))
    { // P6 - M -> m(){C;r(E);}      //t6
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
        goto Q1;
    }
    else if ((in[token] == '0') && (STACK[STACK_TOP] == 'E'))
    { // P7 - E -> 0      //t7
        printLineTable("Q1", in, "δ7", "p7", token);
        pop();
        push('0');
        goto Q1;
    }
    else if ((in[token] == '1') && (STACK[STACK_TOP] == 'E'))
    { // P8 - E -> 1      //t8
        printLineTable("Q1", in, "δ8", "p8", token);
        pop();
        push('1');
        goto Q1;
    }
    else if ((in[token] == 'x') && (STACK[STACK_TOP] == 'E'))
    { // P9 - E -> x      //t9
        printLineTable("Q1", in, "δ9", "p9", token);
        pop();
        push('x');
        goto Q1;
    }
    else if ((in[token] == 'y') && (STACK[STACK_TOP] == 'E'))
    { // P10 - E -> y      //t10
        printLineTable("Q1", in, "δ10", "p10", token);
        pop();
        push('y');
        goto Q1;
    }
    else if ((in[token] == '(') && (STACK[STACK_TOP] == 'E'))
    { // P11 - E -> (EXE)      //t11
        printLineTable("Q1", in, "δ11", "p11", token);
        pop();
        push(')');
        push('E');
        push('X');
        push('E');
        push('(');
        goto Q1;
    }
    else if ((in[token] == '+') && (STACK[STACK_TOP] == 'X'))
    { // P12 - X -> +      //t12
        printLineTable("Q1", in, "δ12", "p12", token);
        pop();
        push('+');
        goto Q1;
    }
    else if ((in[token] == '-') && (STACK[STACK_TOP] == 'X'))
    { // P13 - X -> -      //t13
        printLineTable("Q1", in, "δ13", "p13", token);
        pop();
        push('-');
        goto Q1;
    }
    else if ((in[token] == '*') && (STACK[STACK_TOP] == 'X'))
    { // P14 - X -> *      //t14
        printLineTable("Q1", in, "δ14", "p14", token);
        pop();
        push('*');
        goto Q1;
    }
    else if ((in[token] == '/') && (STACK[STACK_TOP] == 'X'))
    { // P15 - X -> /      //t15
        printLineTable("Q1", in, "δ15", "p15", token);
        pop();
        push('/');
        goto Q1;
    }
    else if ((in[token] == 'h') && (STACK[STACK_TOP] == 'C'))
    { // P16 - C -> h=E      //t16
        printLineTable("Q1", in, "δ16", "p16", token);
        pop();
        push('E');
        push('=');
        push('h');
        goto Q1;
    }
    else if ((in[token] == 'i') && (STACK[STACK_TOP] == 'C'))
    { // P17 - C -> i=E      //t17
        printLineTable("Q1", in, "δ17", "p17", token);
        pop();
        push('E');
        push('=');
        push('i');
        goto Q1;
    }
    else if ((in[token] == 'j') && (STACK[STACK_TOP] == 'C'))
    { // P18 - C -> j=E      //t18
        printLineTable("Q1", in, "δ18", "p18", token);
        pop();
        push('E');
        push('=');
        push('j');
        goto Q1;
    }
    else if ((in[token] == 'k') && (STACK[STACK_TOP] == 'C'))
    { // P19 - C -> k=E      //t19
        printLineTable("Q1", in, "δ19", "p19", token);
        pop();
        push('E');
        push('=');
        push('k');
        goto Q1;
    }
    else if ((in[token] == 'z') && (STACK[STACK_TOP] == 'C'))
    { // P20 - C -> z=E      //t20
        printLineTable("Q1", in, "δ20", "p20", token);
        pop();
        push('E');
        push('=');
        push('z');
        goto Q1;
    }
    else if ((in[token] == '(') && (STACK[STACK_TOP] == 'C'))
    { // P21 - C -> (EXE)      //t21
        printLineTable("Q1", in, "δ21", "p21", token);
        pop();
        push(')');
        push('E');
        push('X');
        push('E');
        push('(');
        goto Q1;
    }
    else if ((in[token] == 'w') && (STACK[STACK_TOP] == 'C'))
    { // P22 - C -> w(E){C;}      //t22
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
        goto Q1;
    }
    else if ((in[token] == 'f') && (STACK[STACK_TOP] == 'C'))
    { // P23 - C -> f(E){C;}      //t23
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
        goto Q1;
    }
    else if ((in[token] == 'o') && (STACK[STACK_TOP] == 'C'))
    { // P24 - C -> o(E;E;E){C;}      //t24
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