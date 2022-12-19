#include <stdio.h>
#include <stdlib.h>

int S();
int M();
int G();
int N();
int F();
int W();
int O();
int E();
int X();
int C();
int err(int error, char *value, char function);
void lex();

FILE *file_IN;
char token = '\0';
int qtd_entries;

main()
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

    for (int i = 0; i < qtd_entries; i++)
    {
    }
    fclose(file_IN);
}
// S-> M | G M | F M G
int S()
{
    if (token == 'm')
    {
        M();
    }
    else if (token == 'g')
    {
        G();
        M();
    }
    else if (token == 'f')
    {
        F();
        G();
        M();
    }
    else
    {
        erro(1, 'm | g | f', 'S');
    }
}
// M → m(){ C; r(E); }
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
                                            err(1, "}", "M");
                                        }
                                    }
                                    else
                                    {
                                        err(2, ";", "M");
                                    }
                                }
                                else
                                {
                                    err(3, ")", "M");
                                }
                            }
                            else
                            {
                                err(4, "(", "M");
                            }
                        }
                        else
                        {
                            err(5, "r", "M");
                        }
                    }
                    else
                    {
                        err(6, ";", "M");
                    }
                }
                else
                {
                    err(7, "{", "M");
                }
            }
            else
            {
                err(8, ")", "M");
            }
        }
        else
        {
            err(9, "(", "M");
        }
    }
    else
    {
        err(10, "m", "M");
    }
}
// G → g(){ C; r(E); }
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
                                            err(1, "}", "G");
                                        }
                                    }
                                    else
                                    {
                                        err(2, ";", "G");
                                    }
                                }
                                else
                                {
                                    err(3, ")", "G");
                                }
                            }
                            else
                            {
                                err(4, "(", "G");
                            }
                        }
                        else
                        {
                            err(5, "r", "G");
                        }
                    }
                    else
                    {
                        err(6, ";", "G");
                    }
                }
                else
                {
                    err(7, "{", "G");
                }
            }
            else
            {
                err(8, ")", "G");
            }
        }
        else
        {
            err(9, "(", "G");
        }
    }
    else
    {
        err(10, "g", "G");
    }
}
// N → n(){ C; r(E); }
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
                                            err(1, "}", "N");
                                        }
                                    }
                                    else
                                    {
                                        err(2, ";", "N");
                                    }
                                }
                                else
                                {
                                    err(3, ")", "N");
                                }
                            }
                            else
                            {
                                err(4, "(", "N");
                            }
                        }
                        else
                        {
                            err(5, "r", "N");
                        }
                    }
                    else
                    {
                        err(6, ";", "N");
                    }
                }
                else
                {
                    err(7, "{", "N");
                }
            }
            else
            {
                err(8, ")", "N");
            }
        }
        else
        {
            err(9, "(", "N");
        }
    }
    else
    {
        err(10, "n", "N");
    }
}
// F →f(E){ C; }
int F()
{
    if (token == '(')
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
                        erro(1, "}", 'F');
                    }
                }
                else
                {
                    erro(2, ";", 'F');
                }
            }
            else
            {
                erro(3, "{", 'F');
            }
        }
        else
        {
            erro(4, ")", 'F');
        }
    }
    else
    {
        erro(5, "(", 'F');
    }
    return (1);
}
// W →w(E){ C; }
int W()
{
    if (token == '(')
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
                        lex();
                    else
                        erro(1, "}", 'W');
                }
                else
                    erro(2, ";", 'W');
            }
            else
                erro(3, "{", 'W');
        }
        else
            erro(4, ")", 'W');
    }
    else
        erro(5, "(", 'W');

    return (1);
}
// O → o(E; E; E){ C; }
int O()
{
    if (token == '(')
    {
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
                                erro(1, "}", 'O');
                            }
                        }
                        else
                        {
                            erro(2, ";", 'O');
                        }
                    }
                    else
                    {
                        erro(3, "{", 'O');
                    }
                }
                else
                {
                    erro(4, ")", 'O');
                }
            }
            else
            {
                erro(5, ";", 'O');
            }
        }
        else
        {
            erro(6, ";", 'O');
        }
    }
    else
    {
        erro(7, "(", 'O');
    }

    return (1);
}
// E → 0 | 1 | x | y | (EXE)
int E()
{
    if (token == '0' || token == '1' || token == 'x' || token == 'y')
        lex();
    else if (token == '(')
    {
        lex();
        E();
        X();
        E();
        if (token == ')')
        {
            lex();
        }
        else
        {
            erro(1, 'E', ")");
        }
    }
    else
        erro(2, "0/1/x/y/(", 'E');
    return (1);
}
// X → + | - | * | /
int X()
{
    if (token == '+' || token == '-' || token == '*' || token == '/')
    {
        lex();
    }
    else
    {
        erro(1, "+ | - | * | /", 'X');
    }
    return (1);
}
// C → h=E | i=E | j=E | k=E | z=E | (EXE) | w(E){ C; } | f(E){ C; } | o(E; E; E){ C; }
int C()
{

    if (token == 'h' || token == 'i' || token == 'j' || token == 'k' || token == 'z' ||)
    {
        lex();
        if (token == '=')
        {
            lex();
            E();
        }
        else
        {
            erro(1, '=', 'C');
        }
    }
    else if (token == '(')
    {
        lex();
        E();
        X();
        E();
        if (token == ')')
            lex();
        else
            erro(2, ")", 'C');
    }
    else if (token == 'w')
    {
        lex();
        W();
    }
    else if (token == 'f')
    {
        lex();
        F();
    }
    else if (token == 'o')
    {
        lex();
        O();
    }
    else
    {
        erro(3, "h | i | j | k | z | ( | w | f | o", 'C');
    }
    return (1);
}

void erro(int error, char *on_value, char on_function)
{
    printf("----------------------------------------------------------------");
    printf("\tErro: [%d] \t Função: '%c' \n", err, on_function);
    printf("\tEra esperado: '%s \t foi lido: '%c'\n", on_value, token);
    printf("----------------------------------------------------------------");
    exit(1);
}
