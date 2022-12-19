#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int token;
    char word[64] = "";

    INICIO:
        scanf("%d", &token);

        if (token == 0){
            strcat(word, "0");
        }
        else if (token == 1){
            strcat(word, "1");
        }
        else{
            goto FIM;  
        }

        goto INICIO;

    FIM:
        printf ("Result: '%s'\n", word);
        return 0;
}
