#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#define MAX_WORD_LEN 50
#define MAX_MEANING_LEN 200
#define MAX_WORDS 100

typedef struct {
    char word[MAX_WORD_LEN];
    char meaning[MAX_MEANING_LEN];
} WordEntry;

int loadWords(const char *filename, WordEntry words[], int maxWords);
void print_correct(const char *msg);
void print_wrong(const char *msg);
void print_warning(const char *msg);
#endif
