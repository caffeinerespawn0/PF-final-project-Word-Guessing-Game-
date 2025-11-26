#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include "game_logic.h"

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

int batteryRow = 10;
int wordRow = 12;
int attemptsRow = 14;
int inputRow = 16;
int messageRow = 18; // for feedback messages like correct/wrong/already guessed

//(x=column, y=row)
void setCursor(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Show battery bar
void showBattery(int attempts, int total, int x, int y) {
    setCursor(x, y); // fixed position
    int segments = 10;
    int filled = (attempts * segments) / total;
    char *color;

    if ((float)attempts / total > 0.6) color = GREEN;
    else if ((float)attempts / total > 0.3) color = YELLOW;
    else color = RED;

    printf("Chances: [");
    printf("%s", color);
    for (int i = 0; i < filled; i++) putchar('\xdb');   // full block
    printf("%s", RESET);
    for (int i = filled; i < segments; i++) putchar('-'); // empty segments
    printf("]  (%d/%d)  ", attempts, total); // extra spaces to overwrite
}


void printDisplay(const char *display, int row) {
    int consoleWidth = 80;
    int len = strlen(display);
    int padding = (consoleWidth - len*2) / 2; // *2 for spacing
    setCursor(padding, row);
    for (int i = 0; i < len; i++) printf("%c ", display[i]);
}

//Beeps
void beep_wrong(){
    Beep(400, 150);
}
void beep_right(){
    Beep(800, 120);
}
void beep_win(){
    Beep(1200, 200);
}

void playGame(WordEntry words[], int total) {
    srand(time(NULL));
    int idx = rand() % total;
    char *word = words[idx].word;
    int len = strlen(word);
    char display[50];

    for (int i = 0; i < len; i++) display[i] = '_';
    display[len] = '\0';

    int attempts = len + 3;

    const char *wrongMessages[] = {
        "Bro, that letter isn't in the word!",
        "Really? Try another one!",
        "Nah, that's not it. Focus!",
        "Hmm...no luck with that letter!",
        "Oops, not this one!"
    };
    int numWrongMessages = sizeof(wrongMessages) / sizeof(wrongMessages[0]);

    printf("\033[36m===============================================\033[0m\n");
    printf("\033[33m           Guess It or L + Ratio \033[0m\n");
    printf("\033[36m===============================================\033[0m\n\n");
    printf("Hint: %s\n\n", words[idx].meaning);

    // Fixed positions for board
    int batteryRow = 10;
    int wordRow = 12;
    int attemptsRow = 14;

    // Initialize guessed letters
    char guessed[50];
    int guessedCount = 0;

    int inputRow = 16;
    int messageRow = 18;

    while (attempts > 0) {
        showBattery(attempts, len + 3, 0, batteryRow);
        printDisplay(display, wordRow);

        // Display attempts
        setCursor(0, attemptsRow);
        printf("Attempts left: %d   ", attempts); //im adding extra spaces here to overwrite

        // Input
        setCursor(0, inputRow);
        printf("Enter a letter or guess the full word: ");
        char input[50];
        scanf("%s", input);

        // Clear input line for next round
        setCursor(0, inputRow);
        printf("                                                   ");

        // Full word guess
        if (strcasecmp(input, word) == 0) {
            strcpy(display, word);
            setCursor(0, messageRow);
            printf("                                                   "); // clear previous
            setCursor(0, messageRow);
            print_correct("Congratulations! You guessed the word!");
            beep_win();
            break;
        }

        // Single letter guess
        if (strlen(input) != 1) {
            setCursor(0, messageRow);
            printf("                                                   ");
            setCursor(0, messageRow);
            print_warning("Enter a single letter or guess the full word.");
            continue;
        }

        char guess = input[0];
        int correct = 0;
        int alreadyGuessed = 0;

        // Checking if the letter is already guessed
        for (int i = 0; i < guessedCount; i++) {
            if (tolower(guess) == tolower(guessed[i])) {
                alreadyGuessed = 1;
                break;
            }
        }

        if (alreadyGuessed) {
            setCursor(0, messageRow);
            printf("                                                   ");
            setCursor(0, messageRow);
            print_warning("You already guessed that letter!");
            beep_wrong();
            continue;
        }

        // Save guessed letter so that i can display it back to the user
        guessed[guessedCount++] = guess;

        // Check letter in word
        for (int i = 0; i < len; i++) {
            if (tolower(word[i]) == tolower(guess) && display[i] == '_') {
                display[i] = word[i];
                correct = 1;
            }
        }

        // Feedback
        setCursor(0, messageRow);
        printf("                                                   "); // clear old message
        setCursor(0, messageRow);

        if (correct) {
            print_correct("Correct guess!");
            beep_right();
        } else {
            int msgIdx = rand() % numWrongMessages;
            print_wrong(wrongMessages[msgIdx]);
            beep_wrong();
            attempts--;
        }

        // Check if fully guessed
        if (strcmp(display, word) == 0) {
            setCursor(0, messageRow);
            printf("                                                   ");
            setCursor(0, messageRow);
            print_correct("Congratulations! You guessed the word!");
            beep_win();
            printf("\nThe word was %s\n", word);
            break;
        }
    }

    // Out of attempts
    if (strcmp(display, word) != 0) {
        setCursor(0, messageRow);
        printf("                                                   ");
        setCursor(0, messageRow);
        print_wrong("Out of attempts! The word was:");
        printf(" %s\n", word);
    }
}

int main() {
    // Array to hold all words loaded from text files
    WordEntry words[MAX_WORDS];
    int total = 0;

    // Print welcome with colors
    printf(CYAN "===============================================\n" RESET);
    printf(YELLOW "           Guess It or L + Ratio \n" RESET);
    printf(CYAN "===============================================\n\n" RESET);
    printf("Semester-End Project - Programming Fundamentals\n");
    printf("-----------------------------------------------\n");

    // Select difficulty
    printf("Select difficulty:\n");
    printf(GREEN "1. Easy\n" RESET);
    printf(YELLOW "2. Medium\n" RESET);
    printf(RED "3. Hard\n" RESET);

int choice;
scanf("%d", &choice);

    int attempts;
    switch (choice) { 
        case 1: 
        total = loadWords("words/easy.txt", words, MAX_WORDS);
        break;
        case 2:
        total = loadWords("words/medium.txt", words, MAX_WORDS);
        break;
        case 3: total = loadWords("words/hard.txt", words, MAX_WORDS);
        break; 
        default: 
        printf("Invalid choice\n");
        return 0;
        } 
        if (total == 0) { printf("No words loaded. Check your files!\n"); return 0; }
    if(choice==1){
    printf("\nYou are now playing in " GREEN"easy mode \n");
    }
    else if(choice==2){
    printf("\nYou are now playing in " YELLOW"medium mode \n");
    }
    else if(choice==3){
    printf("\nYou are now playing in "RED"hard mode \n");
    }
    system("pause");
    system("cls");
    playGame(words, total);

    return 0;
}
