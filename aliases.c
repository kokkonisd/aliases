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

void print_alias (char * line)
{
    int i = 6;

    while (line[i] != '=') {
        printf("%c", line[i]);
        i++;
    }

    i++;

    printf(" -> ");
    while (i < strlen(line) - 1) {
        printf("%c", line[i]);
        i++;
    }

    printf("\n");
}

void print_aliases (char * filename)
{
    FILE * fp = fopen(filename, "r");

    check(fp != NULL, "File doesn't exist.");

    char * line = NULL;
    size_t len = 0;

    printf("\e[33mAliases in file `%s`:\e[0m\n", filename);
    printf("\e[33m------------------------------------------\e[0m\n");

    while (!feof(fp)) {
        getline(&line, &len, fp);

        if (is_alias(line))
            print_alias(line);
    }

error:
    return;
}

int main (int argc, char *argv[])
{
    glob_t globber;
    int rc = 0;
    int flags = 0;

    flags |= GLOB_TILDE;
    rc = glob(BASHRC, flags, NULL, &globber);

    if (rc == 0) {
        print_aliases(globber.gl_pathv[0]);
        printf("\n");
    }

    rc = glob(ZSHRC, flags, NULL, &globber);
    if (rc == 0)
        print_aliases(globber.gl_pathv[0]);

    return 0;
}
