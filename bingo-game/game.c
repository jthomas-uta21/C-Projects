#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5

// Function declarations
void initializeCard(int card[SIZE][SIZE]);
void printCard(int card[SIZE][SIZE]);
int checkWin(int card[SIZE][SIZE]);

int main() {
    // Seed for random number generation
    srand(time(NULL));

    // Bingo card
    int bingoCard[SIZE][SIZE];

    // Initialize Bingo card
    initializeCard(bingoCard);

    // Main game loop
    while (1) {
        system("clear");  // Clear the console (for Linux/macOS, use "cls" for Windows)

        // Display Bingo card
        printf("Bingo Card:\n");
        printCard(bingoCard);

        // Check for win
        if (checkWin(bingoCard)) {
            printf("Bingo! You won!\n");
            break;
        }

        // Call a random number (1-25)
        int calledNumber = rand() % 25 + 1;
        printf("\nCalled Number: %d\n", calledNumber);

        // Mark called number on the card
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (bingoCard[i][j] == calledNumber) {
                    bingoCard[i][j] = 0; // Mark the number as called
                }
            }
        }

        // Wait for user input to continue
        printf("\nPress Enter to continue...");
        getchar();
    }

    return 0;
}

// Function to initialize the Bingo card with random numbers
void initializeCard(int card[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int num;
            do {
                num = rand() % 25 + 1;
            } while (card[i][j] == num);

            card[i][j] = num;
        }
    }
}

// Function to print the Bingo card
void printCard(int card[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%2d ", card[i][j]);
        }
        printf("\n");
    }
}

// Function to check for a win
int checkWin(int card[SIZE][SIZE]) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        int rowSum = 0;
        int colSum = 0;
        for (int j = 0; j < SIZE; j++) {
            rowSum += card[i][j];
            colSum += card[j][i];
        }
        if (rowSum == 0 || colSum == 0) {
            return 1; // Win
        }
    }

    // Check diagonals
    int diag1 = 0;
    int diag2 = 0;
    for (int i = 0; i < SIZE; i++) {
        diag1 += card[i][i];
        diag2 += card[i][SIZE - i - 1];
    }
    if (diag1 == 0 || diag2 == 0) {
        return 1; // Win
    }

    return 0; // No win yet
}
