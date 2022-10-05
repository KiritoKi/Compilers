#include <stdio.h>

int Q1(char *X, int i)
{

    if (X[i] == '\n')                    // verifica se chegou ao final
        return 1;                        // accept
    else if (X[i] == '1' || X[i] == '0') // verifica se é 1 ou 0
        return Q1(X, i + 1);             // chama a função para verificar o próximo bit da linha
    else
        return 0; // error
}

int Q0(char *X)
{
    if (X[0] == '0' || X[0] == '1')
        return Q1(X, 1);
    else
        return 0; // error
}

void automato(char *line)
{
    int result;

    result = Q0(line);

    if (result == 1)
        printf("Accept : %s ", line);
    else if (result == 0)
        printf("Reject : %s ", line);

    printf("\n\n\n");
}

void main(int argc, char *argv[])
{
    FILE *archive;
    char line[100];
    char *result;
    int i;

    // Abre um arquivo TEXTO para LEITURA
    archive = fopen("ArqTeste.txt", "rt");

    if (archive == NULL) // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return;
    }

    i = 1;
    while (!feof(archive))
    {
        // Lê uma linha (inclusive com o '\n')
        result = fgets(line, 100, archive);

        printf("Linha %d  -> ", i);

        if (result) // Se foi possível ler
            automato(line);
        i++;
    }
    fclose(archive);
}