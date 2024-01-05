#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROWS 3
#define COLS 3
#define MAX_PLAYERS 7

// Define the symbols and their respective counts and values
#define SYMBOL_COUNT 4
#define SYMBOLS          { '$', '#', '&', '%' }
#define SYMBOLS_VALUES    { 2,   4,   6,   8 }

// Structure to store player data
struct Player {
    char name[50];
    float balance;
    int gamesPlayed;
    float totalWinnings;
    float highestSingleWin;
    float lowestSingleWin;
    float timePlayed; // Time played in minutes
    struct Player* next;
};

// Global pointer to the top of the player stack
struct Player* topPlayer = NULL;

// Function to push a new player onto the stack with a starting balance of 1000
void pushPlayer(const char* name) {
    struct Player* newPlayer = (struct Player*)malloc(sizeof(struct Player));
    strcpy(newPlayer->name, name);
    newPlayer->balance = 1000; // Set starting balance to 1000
    newPlayer->gamesPlayed = 0;
    newPlayer->totalWinnings = 0;
    newPlayer->highestSingleWin = 0;
    newPlayer->lowestSingleWin = 0;
    newPlayer->timePlayed = 0;
    newPlayer->next = topPlayer;
    topPlayer = newPlayer;
}

// Function to find a player by name (linear search)
struct Player* findPlayerByName(const char* name) {
    struct Player* current = topPlayer;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Player not found
}

// Function to remove a player by name
void removePlayer(const char* name) {
    struct Player* current = topPlayer;
    struct Player* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                topPlayer = current->next;
            }
            free(current);
            printf("\n\n\t\t\t\t\tPlayer %s removed successfully!\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("\n\n\t\t\t\t\tPlayer %s not found.\n", name);
}

// Function to display current players
void displayCurrentPlayers() {
    struct Player* current = topPlayer;
    if (current == NULL) {
        printf("\n\n\t\t\t\t\tNo players registered.\n");
        return;
    }

    printf("\n\n\t\t\t\t\tCurrent Players:\n");
    while (current != NULL) {
        printf("\n\n\t\t\t\t\t- %s (Balance: PHP %.2f)\n", current->name, current->balance);
        current = current->next;
    }
}

// Function to spin the slot machine and return the reels
void spin(char reels[COLS][ROWS]) {
    char symbols[] = SYMBOLS; // Symbols pool
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            int randomIndex = rand() % SYMBOL_COUNT; // Generate a random index
            reels[i][j] = symbols[randomIndex]; // Assign a symbol to the reel
        }
    }
}

// Function to check if the user won and calculate winnings
float getWinnings(char rows[ROWS][COLS], float bet, int lines) {
    float winnings = 0;

    for (int row = 0; row < lines; row++) {
        char symbols[COLS];
        int allSame = 1;

        for (int col = 0; col < COLS; col++) {
            symbols[col] = rows[row][col];
            if (col > 0 && symbols[col] != symbols[0]) {
                allSame = 0;
                break;
            }
        }

        if (allSame) {
            char symbol = symbols[0];
            switch (symbol) {
                case '$':
                    winnings += bet * 2;
                    break;
                case '#':
                    winnings += bet * 4;
                    break;
                case '&':
                    winnings += bet * 6;
                    break;
                case '%':
                    winnings += bet * 8;
                    break;
                default:
                    winnings += 0; // Default value for unknown symbols
                    break;
            }
        }
    }
    return winnings;
}

// Function to transpose the reels
void transpose(char reels[COLS][ROWS], char transposed[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            transposed[i][j] = reels[j][i];
        }
    }
}

// Function to print the rows
void printRows(char rows[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("\t\t\t\t\t%c", rows[i][j]);
        }
        printf("\n\n\n");
    }
}

// Function to play the slot machine game
void playGame() {
    char playerName[50];
    printf("\n\n\t\t\t\t\tEnter your name: ");
    scanf("%s", playerName);

    struct Player* currentPlayer = findPlayerByName(playerName);
    if (currentPlayer == NULL) {
        printf("\n\n\t\t\t\t\tPlayer not found! Please input a registered name!\n");
        return;
    }

    while (1) {
        printf("\n\n\t\t\t\t\tYou have a starting balance of PHP 1000. Your current balance is PHP %.2f\n", currentPlayer->balance);

        // Check if the player's balance is zero or negative
        if (currentPlayer->balance <= 0) {
            printf("\n\n\t\t\t\t\tYou are out of funds. Cannot play anymore.\n");
            break;
        }

        float bet;
        int numberOfLines;

        // Get the bet per line
        while (1) {
            printf("\n\n\t\t\t\t\tEnter the bet per line in PHP (Maximum bet: PHP %.2f): ", currentPlayer->balance);
            if (scanf("%f", &bet) != 1 || bet <= 0 || bet > currentPlayer->balance) {
                printf("\n\n\t\t\t\t\tInvalid bet, please try again!\n");
                while (getchar() != '\n'); // Clear input buffer
            } else {
                break;
            }
        }

        // Get the number of lines to bet on
        while (1) {
            printf("\n\n\t\t\t\t\tEnter the number of lines to bet on (1-3): ");
            if (scanf("%d", &numberOfLines) != 1 || numberOfLines <= 0 || numberOfLines > 3) {
                printf("\n\n\t\t\t\t\tInvalid number of lines, please try again!\n");
                while (getchar() != '\n'); // Clear input buffer
            } else {
                break;
            }
        }

        // Calculate the total bet amount
        float totalBet = bet * numberOfLines;

        // Check if the player has sufficient balance for the bet
        if (totalBet > currentPlayer->balance) {
            printf("\n\n\t\t\t\t\tInsufficient balance to place this bet. Please try again with a lower bet.\n");
            continue; // Restart the loop
        }

        // Deduct the total bet amount from the player's balance
        currentPlayer->balance -= totalBet;

        char reels[COLS][ROWS];
        spin(reels);

        char rows[ROWS][COLS];
        transpose(reels, rows);
        printRows(rows);

        float winnings = getWinnings(rows, bet, numberOfLines);

        // Update Highest Single Win
        if (winnings > currentPlayer->highestSingleWin) {
            currentPlayer->highestSingleWin = winnings;
        }

        // Update Lowest Single Win
        if (currentPlayer->lowestSingleWin == 0 || winnings < currentPlayer->lowestSingleWin) {
            currentPlayer->lowestSingleWin = winnings;
        }

        // Add the winnings to the player's balance
        currentPlayer->balance += winnings;

        currentPlayer->gamesPlayed++;
        currentPlayer->totalWinnings += winnings;

        printf("\n\n\t\t\t\t\tYou won PHP %.2f\n", winnings);
        printf("\n\n\t\t\t\t\tYour new balance is PHP %.2f\n", currentPlayer->balance);

        // Update Time Played (assuming 5 minutes per game)
        currentPlayer->timePlayed += 5.0;

        char playAgain;
        printf("\n\n\t\t\t\t\tDo you want to play again? (y/n): ");
        scanf(" %c", &playAgain);
        if (playAgain == 'n' || playAgain == 'N') {
            break; // Exit the game loop if the player doesn't want to play anymore
        }
    }
}

// Function to display player statistics
void displayPlayerStats() {
    char playerName[50];
    printf("\n\n\t\t\t\t\tEnter the player's name: ");
    scanf("%s", playerName);

    struct Player* player = findPlayerByName(playerName);
    if (player == NULL) {
        printf("\n\n\t\t\t\t\tPlayer not found. Please enter a registered player's name!\n");
        char returnToMenu;
        printf("\n\n\t\t\t\t\tReturn to the menu? (y/n): ");
        scanf(" %c", &returnToMenu);
        if (returnToMenu == 'n' || returnToMenu == 'N') {
            displayPlayerStats(); // Stay in player stats menu
        }
    } else {
        printf("\n\n\t\t\t\t\tPlayer: %s\n", player->name);
        printf("\t\t\t\t\tBalance: PHP %.2f\n", player->balance);
        printf("\t\t\t\t\tGames Played: %d\n", player->gamesPlayed);
        printf("\t\t\t\t\tTotal Winnings: PHP %.2f\n", player->totalWinnings);

        // Calculate and display Win/Loss Ratio
        if (player->gamesPlayed > 0) {
            float winLossRatio = (float)player->gamesPlayed / (player->gamesPlayed - (player->totalWinnings / 1000));
            printf("\t\t\t\t\tWin/Loss Ratio: %.2f\n", winLossRatio);
        } else {
            printf("\t\t\t\t\tWin/Loss Ratio: N/A (No games played)\n");
        }

        // Calculate and display Average Winnings per Game
        if (player->gamesPlayed > 0) {
            float avgWinningsPerGame = player->totalWinnings / player->gamesPlayed;
            printf("\t\t\t\t\tAverage Winnings per Game: PHP %.2f\n", avgWinningsPerGame);
        } else {
            printf("\t\t\t\t\tAverage Winnings per Game: N/A (No games played)\n");
        }

        // Display Highest Single Win
        printf("\t\t\t\t\tHighest Single Win: PHP %.2f\n", player->highestSingleWin);

        // Display Lowest Single Win
        printf("\t\t\t\t\tLowest Single Win: PHP %.2f\n", player->lowestSingleWin);

        // Display Time Played
        printf("\t\t\t\t\tTime Played: %.2f minutes\n", player->timePlayed);

        char returnToMenu;
        printf("\n\n\t\t\t\t\tReturn to the menu? (y/n): ");
        scanf(" %c", &returnToMenu);
        if (returnToMenu == 'n' || returnToMenu == 'N') {
            displayPlayerStats(); // Stay in player stats menu
        }
    }
}

int main() {
    srand(time(NULL)); // Seed the random number generator
    int numPlayers = 0;
    while (1) {
        printf("\n\t\t\t\t\tSlot Machine Program\n");
        printf("\t\t\t\t\t1. Register/Select Player\n");
        printf("\t\t\t\t\t2. Play Slot Machine\n");
        printf("\t\t\t\t\t3. Display Player Stats\n");
        printf("\t\t\t\t\t4. Display Current Players\n");
        printf("\t\t\t\t\t5. Remove Player\n");
        printf("\t\t\t\t\t6. Quit\n");

        int choice;
        printf("\t\t\t\t\tEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("\n\n\t\t\t\t\tInvalid input, please try again!\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                {
                    char playerName[50];
                    printf("\n\n\t\t\t\t\tEnter your name: ");
                    scanf("%s", playerName);

                    if (findPlayerByName(playerName) != NULL) {
                        printf("\n\n\t\t\t\t\tPlayer already registered!\n");
                    } else if (numPlayers >= MAX_PLAYERS) {
                        printf("\n\n\t\t\t\t\tMaximum number of players reached!\n");
                    } else {
                        pushPlayer(playerName);
                        numPlayers++; // Increment the number of registered players
                        printf("\n\n\t\t\t\t\tPlayer registered successfully!\n");
                        printf("\n\n\t\t\t\t\tYou have a starting balance of PHP 1000.\n");
                    }
                }
                break;
            case 2:
                playGame();
                break;
            case 3:
                displayPlayerStats();
                break;
            case 4:
                displayCurrentPlayers();
                break;
            case 5:
                {
                    char playerName[50];
                    printf("\n\n\t\t\t\t\tEnter the name of the player to remove: ");
                    scanf("%s", playerName);
                    removePlayer(playerName);
                }
                break;
            case 6:
                printf("\n\n\t\t\t\t\tThank you for playing!\n\t\t\t\t\tApplication Developed by:\n\t\t\t\t\tJoshua Uy&Jake Perez");
                exit(0);
            default:
                printf("\n\n\t\t\t\t\tInvalid choice, please select a valid option!\n");
                break;
        }
    }

    return 0;
}
