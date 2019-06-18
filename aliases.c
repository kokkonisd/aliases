/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#include "aliases.h"

int is_alias (char * line)
{
    char * temp = "alias";
    int i = 0;

    for (i = 0; i < strlen(temp); i++)
        if (temp[i] != line[i]) return 0;

    return 1;
}

void print_aliases (char * filename)
{
    FILE * fp = fopen(filename, "r");

    check(fp != NULL, "File doesn't exist.");

    char * line = NULL;
    size_t len = 0;

    while (!feof(fp)) {
        getline(&line, &len, fp);

        if (is_alias(line))
            printf("%s\n", line);
    }

error:
    return;
}

int main (int argc, char *argv[])
{
    print_aliases("/Users/kokkonisd/.zshrc");

    return 0;
}
