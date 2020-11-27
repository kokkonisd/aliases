/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#ifndef __aliases_h__
#define __aliases_h__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <glob.h>
#include <stdbool.h>
#include <unistd.h>
#include <regex.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr,\
        "\e[91m[aliases] " M "\e[0m\n", ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) {\
    log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define VERSION "1.2"
#define VERSION_WIDTH 6
#define LATEST_VERSION "curl -s https://github.com/kokkonisd/aliases/releases/latest | "\
                       "grep -Eo \"/tag/(.+)>\" | "\
                       "head -c 9 | "\
                       "tail -c 3"

#define UPDATE_COMMAND "TMPDWN=$(curl -s https://api.github.com/repos/kokkonisd/aliases/releases/latest | "\
                       "grep \"tag_name\" | "\
                       "awk '{"\
                           "print \"https://github.com/kokkonisd/aliases/releases/download/\" "\
                           "substr($2, 2, length($2)-3) "\
                           "\"/aliases-\" "\
                           "substr($2, 3, length($2)-4) "\
                           "\".tar.gz\""\
                        "}') && "\
                        "curl -L -s $TMPDWN -o /tmp/aliases_latest.tar.gz && "\
                        "tar xvzf /tmp/aliases_latest.tar.gz -C /tmp/ && "\
                        "cd /tmp/aliases-* && "\
                        "make clean && "\
                        "make uninstall && "\
                        "make install && "\
                        "cd .. && "\
                        "rm -rf aliases-* aliases_latest*"


#define BASHRC "~/.bashrc"
#define ZSHRC "~/.zshrc"
#endif
