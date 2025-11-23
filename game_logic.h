#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

/*
   Bit by Bit – Word Guessing Game
   Semester-End Project
   File: game_logic.h
   Contains structure definitions and function prototypes.
*/

#define MAX_WORD_LEN 50
#define MAX_MEANING_LEN 200
#define MAX_WORDS 100

/* WordEntry holds a word and its meaning.
   Example:
   word: "binary"
   meaning: "A number system using base 2" */
typedef struct {
    char word[MAX_WORD_LEN];
    char meaning[MAX_MEANING_LEN];
} WordEntry;

/* Loads words + meanings from a text file.
   Returns number of words successfully loaded. */
int loadWords(const char *filename, WordEntry words[], int maxWords);
void print_correct(const char *msg);
void print_wrong(const char *msg);
void print_warning(const char *msg);
#endif
