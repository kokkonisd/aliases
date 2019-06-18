#ifndef __aliases_h__
#define __aliases_h__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <glob.h>
#include <stdbool.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr,\
        "\e[91m[aliases] (%s:%s:%d: errno: %s) " M "\e[0m\n", __FUNCTION__, __FILE__,\
        __LINE__, clean_errno(), ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) {\
    log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define VERSION "1.0"

#define BASHRC "~/.bashrc"
#define ZSHRC "~/.zshrc"
#endif
