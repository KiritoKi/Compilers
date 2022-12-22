#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Delimitador para ler a linha do arquivo
//(deve conter no final da cada linha até da ultima DEFAULT = enter)

//-------------------->>Primeira linha do GLC-file deve ser _,_,S<<------
//----------------------->>Favor usar '_' no lugar de 'ε'<<------------------------
//------>>ATENÇÃO NO DELIMITER DE LINHA, DEVE EXISTIR NO FINAL DA LINHA NOS ARQUIVOS<<------------------------

typedef struct No{
    char token;
    int index_tree;
    int children;
    int visited;
    int hash;
} Ntree;

typedef struct Node{
    char token;
    int index_tree;
} Nsyntax;

const int tam_tree = 50000;

#define TRUE 1
#define FALSE -1
#define STACKSIZE 500
#define DELIMITER '\n'
//File Variable
char lines_input[40][150]; //[<linhas>][<armazenamento por linha>]
FILE *file_IN;
int line;
int qtd_entries;
//Tables Variables
int STACK_TOP = -1;
char STACK[STACKSIZE];
int STEP = -1;
//Tree Variables
int p_count = 0;
int productions[100]; 
int prod;
int end_tree;
int end_abs_tree;
char token = '\0';
int index_w = 0;
int error_founded = FALSE;


// ReadFileFunctions
int readINPUT(FILE *file_IN); // Retorna quantidade de linhas de entradas
//--------< AUTOMATO >--------
void automato(char *in);
//--------< SYNTAX TREE >--------
void start_tree();
void parsing_tree(Ntree *tree, int curr_node);
void get_production(Ntree *tree, int curr_node);
//--------< ABSTRACT TREE  >---------
void start_abstract_tree(Nsyntax *syntax_tree);
void abstract_tree(Nsyntax *syntax_tree, Ntree *tree, int curr_node);
void get_token(Nsyntax *syntax_tree, Ntree *tree, int curr_node);
// -------<PrintFunctions>-------
void printTableHead();
void printTreeHead();
void printLineTable(char *q, char *w, char *d, char *p, int token);

int S();
int M();
int G();
int N();
int E();
int X();
int C();
void lex();
void err(int error, char *on_value, char on_function);



int main(){

    // Abre um arquivo TEXTO para LEITURA
    file_IN = fopen("input_file.txt", "rt");
    if (file_IN == NULL) // Se houver erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return -1;
    }
    qtd_entries = readINPUT(file_IN);
    printf("lines to input: %d\n\n", qtd_entries);

    for (line = 0; line < qtd_entries; line++)
    {
        for (int i = 0; i<100; i++)
            productions[i] = -1;
        index_w = 0;
        prod = 0;
        p_count = 0;
        error_founded = FALSE;
        printf("\n------<Parser>--------\n");
        S();
        
        printf("\n------<Tree Parser>--------\n");
        start_tree();
        //Se depois da analise, o error continuar FALSE, então printa que foi sucesso
        if(error_founded == FALSE)
            printf(" ");
    }
    fclose(file_IN);
    return 1;
}
// Nao ta sendo utilizado ainda <-A UNSUAL PUSH inverte os caracteres a linha antes de dar push---->


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



void start_tree(){
    
    Ntree tree[tam_tree];
    prod = 0;
    for (int i = 0; i < tam_tree; i++) { 
        tree[i].token = ' '; 
        tree[i].visited = FALSE; 
        tree[i].index_tree = -1; 
        tree[i].children = 0; 
    }
    end_tree = 1; 
    tree[0].token = 'S'; 
    tree[0].index_tree = 0;

    parsing_tree(tree, 0);

    Nsyntax syntax_tree[end_tree];
    for (int i = 0; i< end_tree; i++){
        syntax_tree[i].index_tree = tree[i].index_tree;
        syntax_tree[i].token = tree[i].token;
        printf("[%d|%d| %c ]", i, syntax_tree[i].index_tree, syntax_tree[i].token);
        if (i<end_tree-1){ printf(", "); }
    }
    printf("\nArvore abstrata: "); start_abstract_tree(syntax_tree);
}

void parsing_tree(Ntree *tree, int curr_node){
    if (curr_node == 0 && tree[curr_node].visited == TRUE) 
        return;
    int begin_array = FALSE;
    for (int i = 0; i < end_tree || (tree[i].index_tree == tree[curr_node].index_tree*12+1); i++)
        if (tree[curr_node].index_tree*12+1 == tree[i].index_tree) 
            begin_array = i; 
    //If not has index/production for this node yet
    if ( begin_array == FALSE ) 
        get_production(tree, curr_node);
    else{
        //Verifica os nós daquela cadeia do nó
        for (int i = begin_array; i < begin_array+tree[begin_array].children; i++)
            if (tree[i].visited == FALSE){ 
                parsing_tree(tree, i); 
                return; 
                }
        
        tree[curr_node].visited = TRUE;
    }
    parsing_tree(tree, 0);
}

void get_production(Ntree *tree, int curr_node){
    // P1: S -> M
    if (productions[prod] == 1){
        if (tree[curr_node].token != 'S') 
            return;
        tree[end_tree].token = 'M';
        tree[end_tree].children = 1;
    }
    // P2: S -> GM
    else if(productions[prod] == 2) {
        if (tree[curr_node].token != 'S') 
            return;
        tree[end_tree].token = 'G'; 
        tree[end_tree+1].token = 'M';
        tree[end_tree].children = 2;
    }
    // P3: S -> NGM
    else if(productions[prod] == 3) {
        if (tree[curr_node].token != 'S') 
            return;
        tree[end_tree].token = 'N'; 
        tree[end_tree+1].token = 'G'; 
        tree[end_tree+2].token = 'M';
        tree[end_tree].children = 3;
    }
    // P4: N -> n(){ C; r(E); }
    // P5: G -> g(){ C; r(E); }
    // P6: M -> m() { C; r(E); }
    else if(productions[prod] >= 4 && productions[prod] <= 6) {
        for (int i = 0; i < 12; i++)
            tree[end_tree+i].visited = TRUE;

        if (productions[prod] == 4) {
            if (tree[curr_node].token != 'N') 
                return;
            tree[end_tree].token = 'n';
        }
        else if (productions[prod] == 5) {
            if (tree[curr_node].token != 'G') 
                return;
            tree[end_tree].token = 'g';
        }
        else {
            if (tree[curr_node].token != 'M') 
                return;
            tree[end_tree].token = 'm';
        }
        tree[end_tree+1].token = '('; 
        tree[end_tree+2].token = ')'; 
        tree[end_tree+3].token = '{'; 
        tree[end_tree+4].token = 'C'; 
        tree[end_tree+5].token = ';'; 
        tree[end_tree+6].token = 'r'; 
        tree[end_tree+7].token = '('; 
        tree[end_tree+8].token = 'E';
        tree[end_tree+9].token = ')'; 
        tree[end_tree+10].token = ';'; 
        tree[end_tree+11].token = '}';
        tree[end_tree+4].visited = FALSE; 
        tree[end_tree+8].visited = FALSE;
        tree[end_tree].children = 12;
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[prod] >= 7 && productions[prod] <= 10) {
        if (tree[curr_node].token != 'E') 
            return;
        if (productions[prod] == 7) 
            tree[end_tree].token = '0';
        else if (productions[prod] == 8)
            tree[end_tree].token = '1';
        else if (productions[prod] == 9)
            tree[end_tree].token = 'x';
        else
            tree[end_tree].token = 'y';
        tree[end_tree].visited = TRUE;
        tree[end_tree].children = 1;
    }
    // P11: E -> (EXE)
    // P21: C -> (EXE)
    else if(productions[prod] == 11 || productions[prod] == 21) {
        if(productions[prod] == 11)
            if (tree[curr_node].token != 'E') 
                return;
        else 
            if (tree[curr_node].token != 'C') 
                return; 
        tree[end_tree].token = '('; 
        tree[end_tree+1].token = 'E'; 
        tree[end_tree+2].token = 'X';
        tree[end_tree+3].token = 'E'; 
        tree[end_tree+4].token = ')';
        tree[end_tree].visited = TRUE; 
        tree[end_tree+4].visited = TRUE;
        tree[end_tree].children = 5;
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[prod] >= 12 && productions[prod] <= 15) {
        if (tree[curr_node].token != 'X') 
            return;
        if(productions[prod] == 12)
            tree[end_tree].token = '+';
        else if(productions[prod] == 13)
            tree[end_tree].token = '-';
        else if(productions[prod] == 14)
            tree[end_tree].token = '*';
        else
            tree[end_tree].token = '/';
        tree[end_tree].visited = TRUE;
        tree[end_tree].children = 1;
    }
    // P16: C -> h=E
    // P17: C -> i=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[prod] >= 16 && productions[prod] <= 20) {
        if (tree[curr_node].token != 'C') 
            return;
        if (productions[prod] == 16)
            tree[end_tree].token = 'h';
        else if (productions[prod] == 17)
            tree[end_tree].token = 'i';
        else if (productions[prod] == 18)
            tree[end_tree].token = 'j';
        else if (productions[prod] == 19)
            tree[end_tree].token = 'k';
        else if (productions[prod] == 20)
            tree[end_tree].token = 'z';
        tree[end_tree+1].token = '='; 
        tree[end_tree+2].token = 'E';
        tree[end_tree].visited = TRUE;  
        tree[end_tree+1].visited = TRUE;
        tree[end_tree].children = 3;
    }
    // P22: C -> w(E){ C; }
    // P23: C -> f(E){ C; }
    else if(productions[prod] == 22 || productions[prod] == 23) {
        if (tree[curr_node].token != 'C') 
            return;

        for (int i = 0; i < 8; i++)
            tree[end_tree+i].visited = TRUE;
        if (productions[prod] == 22) 
            tree[end_tree].token = 'w';
        else 
            tree[end_tree].token = 'f';
        tree[end_tree+1].token = '('; 
        tree[end_tree+2].token = 'E'; 
        tree[end_tree+3].token = ')'; 
        tree[end_tree+4].token = '{';
        tree[end_tree+5].token = 'C'; 
        tree[end_tree+6].token = ';'; 
        tree[end_tree+7].token = '}';
        tree[end_tree+2].visited = FALSE; 
        tree[end_tree+5].visited = FALSE;
        tree[end_tree].children = 8;
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[prod] == 24) {
        if (tree[curr_node].token != 'C') 
            return;

        for (int i = 0; i < 12; i++)
            tree[end_tree+i].visited = TRUE;
        tree[end_tree].token = 'o'; 
        tree[end_tree+1].token = '('; 
        tree[end_tree+2].token = 'E'; 
        tree[end_tree+3].token = ';';
        tree[end_tree+4].token = 'E'; 
        tree[end_tree+5].token = ';'; 
        tree[end_tree+6].token = 'E'; 
        tree[end_tree+7].token = ')';
        tree[end_tree+8].token = '{'; 
        tree[end_tree+9].token = 'C'; 
        tree[end_tree+10].token = ';'; 
        tree[end_tree+11].token = '}';
        tree[end_tree+2].visited = FALSE; 
        tree[end_tree+4].visited = FALSE; 
        tree[end_tree+6].visited = FALSE; 
        tree[end_tree+9].visited = FALSE;
        tree[end_tree].children = 12;
    }
    
    for (int i = 0, j = 1; i < tree[end_tree].children; i++, j++)
        tree[end_tree+i].index_tree = tree[curr_node].index_tree*12+j;
        
    end_tree += tree[end_tree].children;
    prod++;
}

void start_abstract_tree(Nsyntax *syntax_tree){
    Ntree tree[end_tree];
    for (int i = 0; i < end_tree; i++) { 
        tree[i].token = ' '; 
        tree[i].visited = FALSE; 
        tree[i].index_tree = 0; 
        tree[i].children = 1; 
        tree[i].hash = 0; 
    }
    end_abs_tree = 1; 
    tree[0].token = 'S';

    abstract_tree(syntax_tree, tree, 0);

    for (int i = 0; i< end_abs_tree; i++){
        printf("[%d | %d | %c]", i, tree[i].index_tree, tree[i].token);
        if (i<end_abs_tree-1){ printf(", "); }
    }
    printf("\n");
}

void abstract_tree(Nsyntax *syntax_tree, Ntree *tree, int curr_node){
    if (curr_node == 0 && tree[curr_node].visited == TRUE) 
        return;
    int begin_array = -1;

    for (int i = 0; i < end_tree || (tree[i].index_tree == tree[curr_node].index_tree*2+1); i++)
        if (tree[i].index_tree == tree[curr_node].index_tree*2+1) 
            begin_array = i; 
    
    if ( begin_array == -1 || tree[curr_node].token == 'X')  
        get_token(syntax_tree, tree, curr_node);
    else{
        for (int i = begin_array; i < end_abs_tree; i++)
            if(tree[i].index_tree == tree[curr_node].index_tree*2+1 || tree[i].index_tree == tree[curr_node].index_tree*2+2) {
                if (tree[i].visited == FALSE){ 
                    abstract_tree(syntax_tree, tree, i); 
                    return; 
                }
            }
        
        tree[curr_node].visited = TRUE;
    }
    abstract_tree(syntax_tree, tree, 0);
}

void get_token(Nsyntax *syntax_tree, Ntree *tree, int curr_node){
    int tokens = 0;
    if (tree[curr_node].token == 'S' || 
    tree[curr_node].token == 'N' || tree[curr_node].token == 'G' || 
    tree[curr_node].token == 'M' || tree[curr_node].token == 'E' || 
    tree[curr_node].token == 'X'|| tree[curr_node].token == 'C')
        while (syntax_tree[tokens].index_tree != tree[curr_node].hash*12+1)
            tokens++; 

    else if (tree[curr_node].token == 'r') 
        while (syntax_tree[tokens].index_tree != tree[curr_node].hash+2) 
            tokens++; 
    
    // S
    if (tree[curr_node].token == 'S'){
        // M
        if (syntax_tree[tokens].token == 'M') {
            tree[curr_node].token = syntax_tree[tokens].token;
            tree[curr_node].hash = syntax_tree[tokens].index_tree;
        }
        //    S
        //   / \
        //  G   M
        else if (syntax_tree[tokens].token == 'G') { 
            tree[curr_node].children = 2;
            // G, M
            for (int i = 0; i < tree[curr_node].children; i++){
                tree[end_abs_tree].token = syntax_tree[tokens+i].token;
                tree[end_abs_tree].hash = syntax_tree[tokens+i].index_tree;
                tree[end_abs_tree++].index_tree = tree[curr_node].index_tree*2+(i+1);
            }
        }
        //    S
        //   / \
        //  N   G
        //     / \
        //    G   M
        else if (syntax_tree[tokens].token == 'N') { tree[curr_node].children = 2;
            int index_w;
            // N
            tree[end_abs_tree].token = syntax_tree[tokens].token;
            tree[end_abs_tree].hash = syntax_tree[tokens].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+1;
            // G pai
            tree[end_abs_tree].token = syntax_tree[tokens+1].token;
            tree[end_abs_tree].hash = syntax_tree[tokens+1].index_tree;
            tree[end_abs_tree].index_tree = 2*tree[curr_node].index_tree+2;
            tree[end_abs_tree].visited = FALSE;
            index_w = tree[end_abs_tree].index_tree;
            tree[end_abs_tree++].children = 2;
            // G filho, M
            for (int i = 1; i <= 2; i++){
                tree[end_abs_tree].token = syntax_tree[tokens+i].token;
                tree[end_abs_tree].hash = syntax_tree[tokens+i].index_tree;
                tree[end_abs_tree++].index_tree = 2*index_w+i;
            }
        }

        else{
            tree[curr_node].visited = TRUE;
            return;
        }
    }
    // NGM, GM, M
    else if (tree[curr_node].token == 'N' || tree[curr_node].token == 'G' || tree[curr_node].token == 'M'){
        int valores[2];
        
        for (int i = tokens; i < tokens+13; i++){
            if (syntax_tree[i].token == 'C'){ valores[0]=i; }
            else if (syntax_tree[i].token == 'r'){ valores[1]=i; break; }
        }

        if (tree[curr_node].token == 'N'){ tree[curr_node].token = 'n'; }
        else if (tree[curr_node].token == 'G'){ tree[curr_node].token = 'g'; }
        else if (tree[curr_node].token == 'M') { tree[curr_node].token = 'm'; }
        else{
            tree[curr_node].visited = TRUE;
            return;
        }
        tree[curr_node].children = 2;
        // C, r
        for (int i = 0; i < tree[curr_node].children; i++){
            tree[end_abs_tree].token = syntax_tree[valores[i]].token;
            tree[end_abs_tree].index_tree = 2*tree[curr_node].index_tree+(i+1);
            tree[end_abs_tree++].hash = syntax_tree[valores[i]].index_tree;
        }
    }
    // r
    else if (tree[curr_node].token == 'r'){
        tree[end_abs_tree].token = syntax_tree[tokens].token;
        tree[end_abs_tree].hash = syntax_tree[tokens].index_tree;
        tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+1;
    }
    // E
    else if (tree[curr_node].token == 'E'){
        // (EXE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // X
            tree[curr_node].token = syntax_tree[tokens+1].token;
            tree[curr_node].hash = syntax_tree[tokens+1].index_tree;
            tree[curr_node].children = 2;
            // E1
            tree[end_abs_tree].token = syntax_tree[tokens].token;
            tree[end_abs_tree].hash = syntax_tree[tokens].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+1;
            // E2
            tree[end_abs_tree].token = syntax_tree[tokens+2].token;
            tree[end_abs_tree].hash = syntax_tree[tokens+2].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+2;
        }
        // E -> 0, 1, x, y
        else if (syntax_tree[tokens].token == '0' || syntax_tree[tokens].token == '1' || 
        syntax_tree[tokens].token == 'x' || syntax_tree[tokens].token == 'y'){
            tree[curr_node].token = syntax_tree[tokens].token;
            tree[curr_node].hash = syntax_tree[tokens].index_tree;
            tree[curr_node].visited = TRUE;
        }

        else{
            tree[curr_node].visited = TRUE;
            return;
        }
    }
    // X
    else if (tree[curr_node].token == 'X'){
        tree[curr_node].token = syntax_tree[tokens].token;
        tree[curr_node].hash = syntax_tree[tokens].index_tree;
    }
    // C
    else if (tree[curr_node].token == 'C'){
        // (EXE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // X
            tree[curr_node].token = syntax_tree[tokens+1].token;
            tree[curr_node].hash = syntax_tree[tokens+1].index_tree;
            tree[curr_node].children = 2;
            // E1
            tree[end_abs_tree].token = syntax_tree[tokens].token;
            tree[end_abs_tree].hash = syntax_tree[tokens].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+1;
            // E2
            tree[end_abs_tree].token = syntax_tree[tokens+2].token;
            tree[end_abs_tree].hash = syntax_tree[tokens+2].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+2;
        }
        // h|i|j|k|z =, E
        else if (syntax_tree[tokens].token == 'h' || syntax_tree[tokens].token == 'i' || syntax_tree[tokens].token == 'j' ||
        syntax_tree[tokens].token == 'k' || syntax_tree[tokens].token == 'z'){
            // =
            tree[curr_node].token = syntax_tree[tokens+1].token;
            tree[curr_node].hash = syntax_tree[tokens+1].index_tree;
            tree[curr_node].children = 2;
            // h|i|j|k|z
            tree[end_abs_tree].token = syntax_tree[tokens].token;
            tree[end_abs_tree].hash = syntax_tree[tokens].index_tree;
            tree[end_abs_tree].index_tree = 2*tree[curr_node].index_tree+1;
            tree[end_abs_tree++].visited = TRUE;
            // E
            tree[end_abs_tree].token = syntax_tree[tokens+2].token;
            tree[end_abs_tree].hash = syntax_tree[tokens+2].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+2;
        }
        // w|f, E, C
        else if (syntax_tree[tokens].token == 'w' || syntax_tree[tokens].token == 'f'){
            int valores[2];
            for (int i = 0; i < end_tree; i++){
                if (syntax_tree[i].index_tree == tree[curr_node].hash*12+1){
                    for (int j = i; j < i+7; j++){
                        if (syntax_tree[j].token == 'E'){ valores[0]=j; }
                        else if (syntax_tree[j].token == 'C'){ valores[1]=j; break; }
                    }
                    break;
                }
            }
            // w
            tree[curr_node].token = syntax_tree[tokens].token;
            tree[curr_node].hash = syntax_tree[tokens].index_tree;
            tree[curr_node].children = 2;
            // E
            tree[end_abs_tree].token = syntax_tree[valores[0]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[0]].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+1;
            // C
            tree[end_abs_tree].token = syntax_tree[valores[1]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[1]].index_tree;
            tree[end_abs_tree++].index_tree = 2*tree[curr_node].index_tree+2;
        }

        //    o
        //   / \
        // E2   E1
        // |   /  \
        // E  E    C
        //        / \
        //       C   E
        else if (syntax_tree[tokens].token == 'o'){
            int valores[4], test = 0;
            for (int i = 0; i < end_tree; i++){
                if (syntax_tree[i].index_tree == tree[curr_node].hash*12+1){
                    for (int j = i; j < i+12; j++){
                        if (syntax_tree[j].token == 'E'){ valores[test++] = j; }
                        else if (syntax_tree[j].token == 'C'){ valores[test] = j; break; }
                    }
                    break;
                }
            }
            int index_w;
            // o
            tree[curr_node].token = syntax_tree[tokens].token;
            tree[curr_node].hash = syntax_tree[tokens].index_tree;
            tree[curr_node].children = 2;
            // E2
            tree[end_abs_tree].token = syntax_tree[valores[1]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[1]].index_tree;
            tree[end_abs_tree].index_tree = 2*tree[curr_node].index_tree+1;
            index_w = tree[end_abs_tree++].index_tree;
            // E
            tree[end_abs_tree].token = syntax_tree[valores[1]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[1]].index_tree;
            tree[end_abs_tree++].index_tree = 2*index_w+1;
            // E1
            tree[end_abs_tree].token = syntax_tree[valores[0]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[0]].index_tree;
            tree[end_abs_tree].index_tree = 2*tree[curr_node].index_tree+2;
            index_w = tree[end_abs_tree++].index_tree;
            // E
            tree[end_abs_tree].token = syntax_tree[valores[0]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[0]].index_tree;
            tree[end_abs_tree++].index_tree = 2*index_w+1;
            // C pai
            tree[end_abs_tree].token = syntax_tree[valores[3]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[3]].index_tree;
            tree[end_abs_tree].index_tree = 2*index_w+2;
            tree[end_abs_tree].children = 2;
            index_w = tree[end_abs_tree++].index_tree;
            // C
            tree[end_abs_tree].token = syntax_tree[valores[3]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[3]].index_tree;
            tree[end_abs_tree++].index_tree = 2*index_w+1;
            // E
            tree[end_abs_tree].token = syntax_tree[valores[2]].token;
            tree[end_abs_tree].hash = syntax_tree[valores[2]].index_tree;
            tree[end_abs_tree++].index_tree = 2*index_w+2;
        }
        
        else{
            tree[curr_node].visited = TRUE;
            return;
        }
    }
}

// S-> P1: M | P2: G M | P3: F M G
int S()
{
    lex();
    if (token == 'm')
    {
        productions[p_count++] = 1;
        M();
    }
    else if (token == 'g')
    {
        productions[p_count++] = 2;
        G();
        M();
    }
    else if (token == 'f')
    {
        productions[p_count++] = 3;
        N();
        G();
        M();
    }
    else
    {
        err(1, "m | g | f", 'S');
    }
    return 0;
}
// P4: M → m(){ C; r(E); }
int M()
{
    if (token == 'm')
    {
        lex();
        if (token == '(')
        {
            lex();
            if (token == ')')
            {
                lex();
                if (token == '{')
                {
                    productions[p_count++] = 4;
                    lex();
                    C();
                    if (token == ';')
                    {
                        lex();
                        if (token == 'r')
                        {
                            lex();
                            if (token == '(')
                            {
                                lex();
                                E();
                                if (token == ')')
                                {
                                    lex();
                                    if (token == ';')
                                    {
                                        lex();
                                        if (token == '}')
                                        {
                                            lex();
                                        }
                                        else
                                        {
                                            err(1, "}", 'M');
                                        }
                                    }
                                    else
                                    {
                                        err(2, ";", 'M');
                                    }
                                }
                                else
                                {
                                    err(3, ")", 'M');
                                }
                            }
                            else
                            {
                                err(4, "(", 'M');
                            }
                        }
                        else
                        {
                            err(5, "r", 'M');
                        }
                    }
                    else
                    {
                        err(6, ";", 'M');
                    }
                }
                else
                {
                    err(7, "{", 'M');
                }
            }
            else
            {
                err(8, ")", 'M');
            }
        }
        else
        {
            err(9, "(", 'M');
        }
    }
    else
    {
        err(10, "m", 'M');
    }
    return 0;
}
// P5: G → g(){ C; r(E); }
int G()
{
    if (token == 'g')
    {
        lex();
        if (token == '(')
        {
            lex();
            if (token == ')')
            {
                lex();
                if (token == '{')
                {
                    productions[p_count++] = 5;
                    lex();
                    C();
                    if (token == ';')
                    {
                        lex();
                        if (token == 'r')
                        {
                            lex();
                            if (token == '(')
                            {
                                lex();
                                E();
                                if (token == ')')
                                {
                                    lex();
                                    if (token == ';')
                                    {
                                        lex();
                                        if (token == '}')
                                        {
                                            lex();
                                        }
                                        else
                                        {
                                            err(1, "}", 'G');
                                        }
                                    }
                                    else
                                    {
                                        err(2, ";", 'G');
                                    }
                                }
                                else
                                {
                                    err(3, ")", 'G');
                                }
                            }
                            else
                            {
                                err(4, "(", 'G');
                            }
                        }
                        else
                        {
                            err(5, "r", 'G');
                        }
                    }
                    else
                    {
                        err(6, ";", 'G');
                    }
                }
                else
                {
                    err(7, "{", 'G');
                }
            }
            else
            {
                err(8, ")", 'G');
            }
        }
        else
        {
            err(9, "(", 'G');
        }
    }
    else
    {
        err(10, "g", 'G');
    }
    return 0;
}
// P6: N → n(){ C; r(E); }
int N()
{
    if (token == 'n')
    {
        lex();
        if (token == '(')
        {
            lex();
            if (token == ')')
            {
                lex();
                if (token == '{')
                {
                    productions[p_count++] = 6;
                    lex();
                    C();
                    if (token == ';')
                    {
                        lex();
                        if (token == 'r')
                        {
                            lex();
                            if (token == '(')
                            {
                                lex();
                                E();
                                if (token == ')')
                                {
                                    lex();
                                    if (token == ';')
                                    {
                                        lex();
                                        if (token == '}')
                                        {
                                            lex();
                                        }
                                        else
                                        {
                                            err(1, "}", 'N');
                                        }
                                    }
                                    else
                                    {
                                        err(2, ";", 'N');
                                    }
                                }
                                else
                                {
                                    err(3, ")", 'N');
                                }
                            }
                            else
                            {
                                err(4, "(", 'N');
                            }
                        }
                        else
                        {
                            err(5, "r", 'N');
                        }
                    }
                    else
                    {
                        err(6, ";", 'N');
                    }
                }
                else
                {
                    err(7, "{", 'N');
                }
            }
            else
            {
                err(8, ")", 'N');
            }
        }
        else
        {
            err(9, "(", 'N');
        }
    }
    else
    {
        err(10, "n", 'N');
    }
    return 0;
}
// E → P7: 0 | P8: 1 | P9: x | P10: y | P11: (EXE)
int E()
{
    if (token == '0'){ 
        productions[p_count++] = 7; 
        lex(); 
    }
    else if (token == '1'){ 
        productions[p_count++] = 8;
        lex();
    }
    else if (token == 'x'){ 
        productions[p_count++] = 9;
        lex();
    }
    else if (token == 'y'){ 
        productions[p_count++] = 10;
        lex();
    }
    else if (token == '('){ 
        productions[p_count++] = 11;
        lex(); 
        E(); 
        X(); 
        E();
        if (token == ')')
            lex();
        else 
            err(1, ")", 'E');
    }
    else 
        err(2, "0/1/x/y/(", 'E');
    return 0;
}
// X → P12: + | P13: - | P14: * | P15: /
int X()
{
    if (token == '+'){ 
        productions[p_count++] = 12; 
        lex(); 
    }
    else if (token == '-'){ 
        productions[p_count++] = 13; 
        lex(); 
        }
    else if (token == '*'){ 
        productions[p_count++] = 14; 
        lex(); 
    }
    else if (token == '/'){ 
        productions[p_count++] = 15; 
        lex(); 
    }
    else err(1, "+|-|*|/", 'X');
    return 0;
}
// C → P16: h=E | P17: i=E | P18: j=E | P19: k=E | P20: z=E | P21: (EXE) | P22: w(E){ C; } | P23: f(E){ C; } | P24: o(E; E; E){ C; }
int C()
{

    if (token == 'h'){ 
        productions[p_count++] = 16; 
        lex(); 
        if (token == '=') { 
            lex(); 
            E(); 
        }
        else 
            err(1, "h", 'C');
    }
    else if (token == 'i'){ 
        productions[p_count++] = 17; 
        lex(); 
        if (token == '=') { 
            lex(); 
            E(); 
            }
        else 
            err(1, "i", 'C');
    }
    else if (token == 'j'){ 
        productions[p_count++] = 18; 
        lex(); 
        if (token == '=') { 
            lex(); 
            E(); }
        else 
            err(1, "j", 'C');
    }
    else if (token == 'k'){ 
        productions[p_count++] = 19; 
        lex(); 
        if (token == '=') { 
            lex(); 
            E(); 
        }
        else 
            err(1, "k", 'C');
    }
    else if (token == 'z'){ 
        productions[p_count++] = 20; 
        lex(); 
        if (token == '=') { 
            lex(); 
            E(); 
            }
        else 
            err(1, "z", 'C');
    }
    else if (token == '(')
    {
        productions[p_count++] = 21;
        lex();
        E();
        X();
        E();
        if (token == ')')
            lex();
        else
            err(2, ")", 'C');
    }
    else if (token == 'w')
    {
        lex();
        if (token == '(')
        {
            productions[p_count++] = 22;
            lex();
            E();
            if (token == ')')
            {
               lex();
                if (token == '{')
                {
                    lex();
                    C();
                    if (token == ';')
                    {
                        lex();
                        if (token == '}')
                            lex();
                       else
                            err(1, "W->}", 'C');
                    }
                    else
                        err(2, "W->;", 'C');
                }
                else
                    err(3, "W->{", 'C');
            }
            else
                err(4, "W->)", 'C');
        }   
        else
            err(5, "W->(", 'C');

        }
    else if (token == 'f')
    {
        lex();
        if (token == '(')
        {
            productions[p_count++] = 23;
            lex();
            E();
            if (token == ')')
            {
                lex();
                if (token == '{')
                {
                    lex();
                    C();
                    if (token == ';')
                    {
                        lex();
                        if (token == '}')
                        {
                            lex();
                        }
                        else
                        {
                            err(1, "F->}", 'C');
                        }
                    }
                    else
                    {
                        err(2, "F->;", 'C');
                    }
                }
                else
                {
                    err(3, "F->{", 'C');
                }
            }
            else
            {
                err(4, "F->)", 'C');
            }
        }
        else
        {
            err(5, "F->(", 'C');
        }
    }
    else if (token == 'o')
    {
        lex();
        if (token == '(')
        {
            productions[p_count++] = 24;
            lex();
            E();
            if (token == ';')
            {
                lex();
                E();
                if (token == ';')
                {
                    lex();
                    E();
                    if (token == ')')
                    {
                        lex();
                        if (token == '{')
                        {
                            lex();
                            C();
                            if (token == ';')
                            {
                                lex();
                                if (token == '}')
                                {
                                    lex();
                                }
                                else
                                {
                                    err(1, "O->}", 'C');
                                }
                            }
                            else
                            {
                                err(2, "O->;", 'C');
                            }
                        }
                        else
                        {
                            err(3, "O->{", 'C');
                        }
                    }
                    else
                    {
                        err(4, "O->)", 'C');
                    }
                }
                else
                {
                    err(5, "O->;", 'C');
                }
            }
            else
            {
                err(6, "O->;", 'C');
            }
        }
        else
        {
            err(7, "O->(", 'C');
        }
    }
    else
    {
        err(3, "h | i | j | k | z | ( | w | f | o", 'C');
    }
    return 0;
}

void lex(){
    do {
        token = lines_input[line][index_w++];
        if (token != '\n')
            printf("%c", token);
    }while(token == ' ');
}

void err(int error, char *on_value, char on_function)
{
    error_founded = TRUE;
    printf("\n----------------------------------------------------------------\n");
    printf("\tErro: [%d] \t Função: '%c' \n", error, on_function);
    printf("\tEra esperado: '%s \t foi lido: '%c'\n", on_value, token);
    printf("----------------------------------------------------------------\n");
    exit(1);
}