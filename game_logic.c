#include <stdio.h>
#include <string.h>
#include "game_logic.h"
#define COLOR_RESET "\033[0m"
#define COLOR_RED   "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"

void print_correct(const char *msg) {
    printf("%s%s%s\n", COLOR_GREEN, msg, COLOR_RESET);
}

void print_wrong(const char *msg) {
    printf("%s%s%s\n", COLOR_RED, msg, COLOR_RESET);
}

void print_warning(const char *msg) {
    printf("%s%s%s\n", COLOR_YELLOW, msg, COLOR_RESET);
}

int loadWords(const char *filename, WordEntry words[], int maxWords) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    int count = 0;
    while (count < maxWords &&
           fscanf(fp, " %49[^:]: %199[^\n]", words[count].word, words[count].meaning) == 2) {

        // Trim leading spaces
        char *w = words[count].word;
        while (*w == ' ') memmove(w, w + 1, strlen(w));

        //Trimming trailing spaces through (\r\n)
        int len = strlen(words[count].word);
        while (len > 0 && (words[count].word[len-1] == ' ' ||
                           words[count].word[len-1] == '\n' ||
                           words[count].word[len-1] == '\r')) {
            words[count].word[len-1] = '\0';
            len--;
        }

        //Trimming trailing spaces in meaning as well
        int mlen = strlen(words[count].meaning);
        while (mlen > 0 && (words[count].meaning[mlen-1] == ' ' || words[count].meaning[mlen-1] == '\n' || words[count].meaning[mlen-1] == '\r')) {
            words[count].meaning[mlen-1] = '\0';
            mlen--;
        }
        count++;
    }

    fclose(fp);
    return count;
}
