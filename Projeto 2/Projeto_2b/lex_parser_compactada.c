#include <stdio.h>
#include <stdlib.h>

int readINPUT(FILE *file_IN);
void start_tree();
void parsing_tree(char *tree, int *visited, int curr_node);
void put_production(char *tree, int *visited, int curr_node);
int S();
int M();
int G();
int N();
int E();
int X();
int C();
void err(int error, char *on_value, char on_function);
void lex();

#define TRUE 1
#define FALSE -1

//File Variable
char lines_input[40][150]; //[<linhas>][<armazenamento por linha>]
FILE *file_IN;
int line;
int qtd_entries;
//
int index = 0;
char token = '\0';
int error_founded = FALSE;
//Tree Variables
int productions[100];
int p_count, prod;

int main()
{
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
        index = 0;
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

void lex(){
    do {
        token = lines_input[line][index++];
        if (token != '\n')
            printf("%c", token);
    }while(token == ' ');
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

void err(int error, char *on_value, char on_function)
{
    error_founded = TRUE;
    printf("\n----------------------------------------------------------------\n");
    printf("\tErro: [%d] \t Função: '%c' \n", error, on_function);
    printf("\tEra esperado: '%s \t foi lido: '%c'\n", on_value, token);
    printf("----------------------------------------------------------------\n");
    exit(1);
}

void start_tree(){
    const int max = 50000;
    char tree[max];
    int visited[max];
    prod = 0;
    for (int i = 0; i < max; i++) { 
        tree[i] = ' '; 
        visited[i] = FALSE; 
    }
    tree[0] = 'S';

    parsing_tree(tree, visited, 0);

    for (int i = 0; i< max; i++){
        if (tree[i] != ' ') 
            printf("%d -> [%c]\n", i, tree[i]); 
    }
    printf("\n");
}

void parsing_tree(char *tree, int *visited, int curr_node){
    printf("-----------------\n");
    printf("%d",prod);
    for(int i = 0; i< 20; i++)
        printf(" >%c< ", tree[i]);
    printf("\n");
    if (curr_node == 0 && visited[curr_node] == TRUE) 
        return;
    if (prod >= p_count) 
        return;
    // --------------------------------------------------------------------
    if (tree[4*curr_node+1] == ' ') { 
        put_production(tree, visited, curr_node); 
        prod++; 
    }
    // --------------------------------------------------------------------
    if (visited[4*curr_node+1] == FALSE){ 
        parsing_tree(tree, visited, 4*curr_node+1); 
    }
    else if (visited[4*curr_node+2] == FALSE){ 
        parsing_tree(tree, visited, 4*curr_node+2); 
    }
    else if (visited[4*curr_node+3] == FALSE){ 
        parsing_tree(tree, visited, 4*curr_node+3); 
    }
    else if (visited[4*curr_node+4] == FALSE){ 
        parsing_tree(tree, visited, 4*curr_node+4); 
    }
    else { 
        visited[curr_node] = TRUE; 
        parsing_tree(tree, visited, 0); 
    }
}

void put_production(char *tree, int *visited, int curr_node){
    //P1: S -> M
    if (productions[prod] == 1){ 
        if (tree[curr_node] == 'S'){
            tree[4*curr_node + 1] = 'M'; 
            tree[4*curr_node + 2] = ' '; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+2] = TRUE; 
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P2: S -> GM
    else if(productions[prod] == 2) {
        if (tree[curr_node] == 'S'){
            tree[4*curr_node + 1] = 'G'; 
            tree[4*curr_node + 2] = 'M'; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P3: S -> NGM
    else if(productions[prod] == 3) {
        if (tree[curr_node] == 'S'){
            tree[4*curr_node + 1] = 'N'; 
            tree[4*curr_node + 2] = 'G'; 
            tree[4*curr_node + 3] = 'M'; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P4: N -> n(){ C; r(E); }
    else if(productions[prod] == 4) {
        if (tree[curr_node] == 'N'){
            tree[4*curr_node + 1] = 'C'; 
            tree[4*curr_node + 2] = 'E'; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P5: G -> g(){ C; r(E); }
    else if(productions[prod] == 5) {
        if (tree[curr_node] == 'G'){
            tree[4*curr_node + 1] = 'C'; 
            tree[4*curr_node + 2] = 'E'; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P6: M -> m() { C; r(E); }
    else if(productions[prod] == 6) {
        if (tree[curr_node] == 'M'){
            tree[4*curr_node + 1] = 'C'; 
            tree[4*curr_node + 2] = 'E'; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[prod] >= 7 && productions[prod] <= 10) {
        if (tree[curr_node] == 'E'){
            if (productions[prod] == 7) 
                tree[4*curr_node + 1] = '0';
            else if (productions[prod] == 8)  
                tree[4*curr_node + 1] = '1'; 
            else if (productions[prod] == 9) 
                tree[4*curr_node + 1] = 'x'; 
            else 
                tree[4*curr_node + 1] = 'y';
            tree[4*curr_node + 2] = ' '; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+1] = TRUE; 
            visited[4*curr_node+2] = TRUE; 
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P11: E -> (EXE)
    else if(productions[prod] == 11) {
        if (tree[curr_node] == 'E'){
            tree[4*curr_node + 1] = 'E'; 
            tree[4*curr_node + 2] = 'X'; 
            tree[4*curr_node + 3] = 'E'; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[prod] >= 12 && productions[prod] <= 15) {
        if (tree[curr_node] == 'X'){
            if(productions[prod] == 12) 
                tree[4*curr_node + 1] = '+';
            else if(productions[prod] == 13) 
                tree[4*curr_node + 1] = '-';
            else if(productions[prod] == 14) 
                tree[4*curr_node + 1] = '*';
            else 
                tree[4*curr_node + 1] = '/';
            tree[4*curr_node + 2] = ' '; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+1] = TRUE; 
            visited[4*curr_node+2] = TRUE; 
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P16: C -> h=E
    // P17: C -> curr_node=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[prod] >= 16 && productions[prod] <= 20) {
        if (tree[curr_node] == 'C'){
            tree[4*curr_node + 1] = 'E'; 
            tree[4*curr_node + 2] = ' '; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+2] = TRUE; 
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P21: C -> (EXE)
    else if(productions[prod] == 21) {
        if (tree[curr_node] == 'C'){
            tree[4*curr_node + 1] = 'E'; 
            tree[4*curr_node + 2] = 'X'; 
            tree[4*curr_node + 3] = 'E'; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P22: C -> w(E){ C; }
    else if(productions[prod] == 22) {
        if (tree[curr_node] == 'C'){
            tree[4*curr_node + 1] = 'E'; 
            tree[4*curr_node + 2] = 'C'; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P23: C -> f(E){ C; }
    else if(productions[prod] == 23) {
        if (tree[curr_node] == 'C'){
            tree[4*curr_node + 1] = 'E'; 
            tree[4*curr_node + 2] = 'C'; 
            tree[4*curr_node + 3] = ' '; 
            tree[4*curr_node + 4] = ' ';
            visited[4*curr_node+3] = TRUE; 
            visited[4*curr_node+4] = TRUE;
        }
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[prod] == 24) {
        if (tree[curr_node] == 'C'){
            tree[4*curr_node + 1] = 'E'; 
            tree[4*curr_node + 2] = 'E'; 
            tree[4*curr_node + 3] = 'E'; 
            tree[4*curr_node + 4] = 'C';
        }
    }
}
