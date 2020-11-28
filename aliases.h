/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#ifndef __aliases_h__
#define __aliases_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <unistd.h>
#include <regex.h>


/**
 * @brief      Logs an info message.
 * 
 * This macro prints a colored info message to `stdout`.
 *
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 */
#define log_info(M, ...) fprintf(stdout, "\e[33m" M "\e[0m", ##__VA_ARGS__);

/**
 * @brief      Logs an error message.
 * 
 * This macro prints a colored error message to `stderr`.
 *
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 */
#define log_err(M, ...) fprintf(stderr, "\e[31m[aliases] " M "\e[0m\n", ##__VA_ARGS__)

/**
 * @brief      Checks a condition and produces an error if false.
 * 
 * This macro checks a condition. If the condition is false, an appropriate error message is printed, and then the
 * function jumps down to its `error` section to handle the error.
 *
 * @param      A     A condition to check.
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 *
 * @return     { description_of_the_return_value }
 */
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); goto error; }


#define VERSION       "1.2.1" /**< The current version of `aliases`. */
#define VERSION_WIDTH 5       /**< The version width (3 digits + 2 dots). */

#ifdef MACOS
#    define GREP "ggrep"
#else
#    define GREP "grep"
#endif

/**
 * @brief       Gets the latest stable version of `aliases` from GitHub.
 * 
 * This command uses curl and grep to grab the latest version of `aliases` from its GitHub repo.
 */
#define LATEST_VERSION_CMD "curl -s https://api.github.com/repos/kokkonisd/aliases/releases/latest | "\
                           GREP " -oP '(?<=\"tag_name\": \"v)[0-9.]+(?=\")'"

#define UPDATE_CMD         "TMPDWN=$(curl -s https://api.github.com/repos/kokkonisd/aliases/releases/latest | "\
                           GREP " -oP '(?<=\"browser_download_url\": \").+(?=\")') &&"\
                           "curl -L -s $TMPDWN -o /tmp/aliases_latest.tar.gz && "\
                           "tar -xvzf /tmp/aliases_latest.tar.gz -C /tmp/ && "\
                           "cd /tmp/aliases-* && "\
                           "make clean && "\
                           "make uninstall && "\
                           "make install && "\
                           "cd .. && "\
                           "rm -rf aliases-* aliases_latest*"


#define ALIAS_KEYWORD "alias"

#define ALIAS_REGEX    "^alias[ \\t]+.+=.+"
#define FUNCTION_REGEX "^.+[ \\t]*\\(.*\\)[ \\t]*\\{"

char * base_files[] = {
    "~/.bashrc",
    "~/.bash_aliases",
    "~/.zshrc",
    "~/.zsh_aliases",
    NULL
};

#endif
