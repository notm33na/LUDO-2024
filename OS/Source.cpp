//22L-7883 Zarmeena Fatima
#pragma once
#include <iostream>
#include <string>
#include <cstdlib> // For rand()
#include <cctype>  // For character checks
#include <vector>
#include <pthread.h>
#include <ctime>
#include <map>
#include <set>
#include <semaphore.h>
#include <windows.h> // For SetConsoleTextAttribute and GetStdHandle

// Define color codes
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15
#ifdef _WIN32
#include <windows.h>
#define sleep(seconds) Sleep((seconds) * 1000) // Convert seconds to milliseconds
#endif

using namespace std;

int current_turn = 0;




// Gameboard
char board[15][15];
// Set colors for each user's path
struct path {
    vector<pair<int, int>> coordinates; // Store path as row-column pairs
};
path yellowPath, greenPath, redPath, bluePath, comppath, safecoords, SAFEspace;
int dicenum[3]={0,0,0};
int numPlayers;  // Number of players
int numTokens;


pthread_mutex_t dice_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex lock for dice roll
pthread_mutex_t board_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex lock for board

pthread_mutex_t game_state_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t turn_cond = PTHREAD_COND_INITIALIZER;





//// Function to acquire token for a player
//void acquireToken(string playerID) {
//    playerSemaphores[playerID].lock();
//}
//
//// Function to release token for a player
//void releaseToken(string playerID) {
//    playerSemaphores[playerID].unlock();
//}
//




void startingtokenposition(int numTokens);

void displaytokenposition();






// Struct Definitions
struct token {
    char name;
    bool home = true;
    bool won = false;
    int r = -1;
    int c = -1;
    int color; // Color attribute (use color codes)
    token* next = nullptr;
};

struct player {
    token* tokens = nullptr;
    string name;
    int color; // Color attribute (use color codes)

    int hitcount = 0;
    int turns = 0;
    bool bot = false;
    string teammate = "Nil";
    player* next = nullptr;

    player() : tokens(nullptr), next(nullptr) {}
};

struct playerslist {
    player* players = nullptr;
    playerslist* next = nullptr;
 pthread_mutex_t mutex;  // Mutex to protect access to the game state
 pthread_cond_t cond_var;
    bool game_over;         // Flag to indicate if the game is over


    playerslist() : players(nullptr), next(nullptr) {
        pthread_mutex_init(&mutex, NULL);
        game_over = 0;
    }
    ~playerslist()
    {
        pthread_mutex_destroy(&mutex);

    }
};




// Singleton Class for Players List Management
class PlayersListManager {
private:
    playerslist* playerList;

    PlayersListManager() : playerList(nullptr) {}

public:



    PlayersListManager(const PlayersListManager&) = delete;
    void operator=(const PlayersListManager&) = delete;

    static PlayersListManager& getInstance() {
        static PlayersListManager instance;
        return instance;
    }

    playerslist*& getPlayersList() {
        if (!playerList) {
            playerList = new playerslist();
            playerList->players = nullptr; // Ensure players pointer is initialized
            playerList->next = nullptr;
        }
        return playerList;
    }





    void addPlayer(player* newPlayer) {
        playerslist*& list = getPlayersList();
        if (!list->players) {
            list->players = newPlayer;
        }
        else {
            player* temp = list->players;
            while (temp->next)
                temp = temp->next;
            temp->next = newPlayer;
        }
        cout << "Added player: " << newPlayer->name << endl;
    }


    void displayPlayers() {
        playerslist* list = getPlayersList();
        while (list) {
            player* currentPlayer = list->players;
            while (currentPlayer) {
                cout << "Player: " << currentPlayer->name
                    //<< " | Team mate: " << currentPlayer->teammate
                    << " | Hitcount: " << currentPlayer->hitcount
                    << " | Turns: " << currentPlayer->turns << endl;
                    //<< " | Bot: " << (currentPlayer->bot ? "Yes" : "No") << endl;
                currentPlayer = currentPlayer->next;
            }
            list = list->next;
        }
    }
};

PlayersListManager& manager = PlayersListManager::getInstance();
int getColorFromCharacter(char c);
void displayboard() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Clear the console before displaying the board
    system("cls");

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            char cell = board[i][j];

            if (cell != '*' && cell != 'S'&& cell != 'B' && cell != 'Y'&& cell != 'R' && cell != 'G'&& cell != 'b' && cell != 'y'&&cell != 'r' && cell != 'g') {
                //// Find the token at the current position
                //playerslist* p = manager.getPlayersList();
                //for (player* pl = p->players; pl != nullptr; pl = pl->next) {
                //    for (token* t = pl->tokens; t != nullptr; t = t->next) {
                //        if (t->r == i && t->c == j) {
                //            // Set the color based on the token's color attribute
                //            SetConsoleTextAttribute(hConsole, t->color);
                //            cout << t->name << " ";
                //            SetConsoleTextAttribute(hConsole, 15); // Reset to default color
                //            break;
                //        }
                //    }
                //}
            }
            else
            {
                // Set the console color based on the character in the cell
                SetConsoleTextAttribute(hConsole, getColorFromCharacter(cell));

                // Print the board cell
            }
            cout << cell << ' ';
        }
        // Reset to default color after printing each row
        SetConsoleTextAttribute(hConsole, WHITE);
        cout << std::endl;
    }

    // Reset color at the end of the display function
    SetConsoleTextAttribute(hConsole, WHITE);
}


bool arealltokenshome(token* tokenn);
bool havealltokenswon(token* tokenn)
{
    int tnum = 0, twon = 0;
    while (tokenn)
    {
        tnum++;
        if (tokenn->won)
            twon++;
        tokenn = tokenn->next;
    }
    return twon == tnum;
}

void* master_thread(void* arg) {
    pthread_t* players = (pthread_t*)arg;
    playerslist* state = manager.getPlayersList();  // Retrieve the game state using the manager instance

    while (!state->game_over) {
        pthread_mutex_lock(&state->mutex);  // Lock the mutex to protect game state

        player* currentPlayer = state->players;

        // Iterate over all players
        int i = 0;
        while (currentPlayer) {
            // If the player has been playing for 20 turns, has no hits, and all tokens are home, cancel the player's thread
            if (currentPlayer->turns >= 20 && arealltokenshome(currentPlayer->tokens) && currentPlayer->hitcount == 0) {
                pthread_cancel(players[i]);
                printf("Master thread canceled player %d.\n", i + 1);
            }

            // Check if the player has won the game (all tokens are home)
            if (havealltokenswon(currentPlayer->tokens)) {
                printf("Player %d has won the game!\n", i + 1);
                state->game_over = true;
                break;  // Break the loop to end the game
            }

            currentPlayer = currentPlayer->next;  // Move to the next player
            i++;  // Increment the player index
        }

        pthread_mutex_unlock(&state->mutex);  // Unlock the mutex

        sleep(1);  // Simulate periodic checking (can be adjusted as needed)
    }

    pthread_exit(NULL);  // Exit the thread
    return NULL;
}




bool isCellInPlayerPath(int pathRow, int pathCol, const path& playerpath) {
    for (const auto& coord : playerpath.coordinates) {
        if (coord.first == pathRow && coord.second == pathCol) {
            return true; // Found a match
        }
    }
    return false; // No match found
}


void movetoken(player* state);

// Dice Roll
void rollDice(player* state) {
    manager.displayPlayers();

    pthread_mutex_lock(&dice_mutex);
    fill(begin(dicenum), end(dicenum), 0); // Reset dice rolls
    int i = 0, targetDiceValue;
    if (state->turns == 0 ) {
        dicenum[i++] = 6;
        dicenum[i++] = (rand() % 5) + 1;
    }

    else if (state->turns == 10 && arealltokenshome(state->tokens) && state->hitcount == 0) {
        dicenum[i++] = 6;
        dicenum[i++] = (rand() % 5) + 1;
    }
     
    else
    {do
    {
        srand(static_cast<unsigned int>(time(0)));
        dicenum[i++] = (rand() % 6) + 1;
    } while (dicenum[i-1] == 6);
    }
    if (count(begin(dicenum), end(dicenum), 6) > 2) {
        cout << "Player " << state->name << " rolled 3 sixes! Turn ends without moving.\n";
        pthread_mutex_unlock(&dice_mutex);
        return;
    }

    cout << "Player " << state->name << " rolled: ";//first
    for (int k = 0; k < 3; k++) {
        if (dicenum[k] != 0) cout << dicenum[k] << " ";
    }
    cout << endl;
    //movetoken(state);
    pthread_mutex_unlock(&dice_mutex);

}





void* player_thread(void* players) {
    
    player* data = (player*)players;
    playerslist* state = manager.getPlayersList();  // Retrieve the game state using the manager instance
    string turn[4] = { "blue","yellow","green", "red"};

    while (!state->game_over) {
        pthread_mutex_lock(&state->mutex);
        //cout << data->name;
        
        while (turn[current_turn] != data->name) {
            pthread_cond_wait(&state->cond_var, &state->mutex);
        }
        rollDice(data);
        //cout << "Player: " << data->name << " rolled dice: ";
       /* for (int k = 0; k < 3; k++) {
            if (dicenum[k] != 0) cout << dicenum[k] << " ";
        }*/
        cout << endl;
        data->turns++;
        movetoken(data);

        // Example: Check for inactivity
        if (data->turns >= 20 && arealltokenshome(data->tokens) && data->hitcount == 0) {
            cout << "Player " << data->name << " removed due to inactivity.\n";
            pthread_exit(NULL);
        }        
        //cout << current_turn << endl;
        current_turn++;
        //cout << current_turn << endl;
        if (numPlayers  == current_turn)
            current_turn = 0;
        pthread_cond_broadcast(&state->cond_var);
        pthread_mutex_unlock(&state->mutex);

        sleep(1);
    }

    pthread_exit(NULL);
    return NULL;
}
void handle_collisions(player* state);
void handle_blocks(player* state);

//void* grid_monitor_thread(void* arg) {
//    playerslist* state = (playerslist*)arg;
//
//    while (!state->game_over) {
//        pthread_mutex_lock(&state->mutex);
//
//        // Handle Collisions
//        handle_collisions(state);
//
//        // Handle Block Formations
//        handle_blocks(state);
//
//        pthread_mutex_unlock(&state->mutex);
//
//        // Sleep to reduce CPU usage
//        sleep(100000); // 100 milliseconds
//    }
//
//    pthread_exit(NULL);
//    return NULL;
//}

void initializePaths() {
    yellowPath.coordinates = { {1, 8}, {2, 8}, {3, 8},
               {4, 8}, {5, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {6, 14}, {7, 14}, {8, 14},
               {8, 13}, {8, 12}, {8, 11}, {8,10}, {8, 9}, {9, 8}, {10, 8}, {11, 8},
               {12, 8}, {13, 8},{14,8}, {14, 7}, {14, 6}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
          {8, 5}, {8, 4}, {8, 3},{8, 2}, {8, 1}, {8, 0}, {7,0} ,
          {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {5, 6}, {4, 6},
               {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 7}, {1, 7}, {2, 7}, {3, 7},
               {4, 7}, {5, 7} };
    greenPath.coordinates = { {8, 13}, {8, 12}, {8, 11}, {8,10}, {8, 9}, {9, 8}, {10, 8}, {11, 8},
               {12, 8}, {13, 8},{14,8}, {14, 7}, {14, 6}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
          {8, 5}, {8, 4}, {8, 3},{8, 2}, {8, 1}, {8, 0}, {7,0} ,
          {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {5, 6}, {4, 6},
               {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 7} ,
          {0, 8},{1,8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 9}, {6, 10}, {6, 11},
               {6, 12}, {6, 13}, {6, 14}, {7, 14}, {7, 13}, {7, 12}, {7, 11}, {7, 10},
               {7, 9} };
    redPath.coordinates = { {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
          {8, 5}, {8, 4}, {8, 3},{8, 2}, {8, 1}, {8, 0}, {7,0} ,
          {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {5, 6}, {4, 6},
               {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 7} ,
          {0, 8},{1,8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 9}, {6, 10}, {6, 11},
               {6, 12}, {6, 13}, {6, 14}, {7, 14},
          {8, 14}, {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9}, {9, 8}, {10, 8},
               {11, 8}, {12, 8}, {13, 8}, {14, 8}, {14, 7}, {13, 7}, {12, 7}, {11, 7},
               {10, 7}, {9, 7} };
    bluePath.coordinates = { {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {5, 6}, {4, 6}, {3, 6},
               {2, 6}, {1, 6}, {0, 6}, {0, 7}, {0, 8}, {1, 8}, {2, 8}, {3, 8},
               {4, 8}, {5, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {6, 14}, {7, 14}, {8, 14},
               {8, 13}, {8, 12}, {8, 11}, {8,10}, {8, 9}, {9, 8}, {10, 8}, {11, 8},
               {12, 8}, {13, 8},{14,8}, {14, 7}, {14, 6}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
          {8, 5}, {8, 4}, {8, 3},{8, 2}, {8, 1}, {8, 0}, {7,0}, {7, 1}, {7, 2}, {7, 3}, {7, 4},
               {7, 5} };
    comppath.coordinates = { {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {5, 6}, {4, 6}, {3, 6},
               {2, 6}, {1, 6}, {0, 6}, {0, 7}, {0, 8}, {1, 8}, {2, 8}, {3, 8},
               {4, 8}, {5, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {6, 14}, {7, 14}, {8, 14},
               {8, 13}, {8, 12}, {8, 11}, {8,10}, {8, 9}, {9, 8}, {10, 8}, {11, 8},
               {12, 8}, {13, 8},{14,8}, {14, 7}, {14, 6}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
          {8, 5}, {8, 4}, {8, 3},{8, 2}, {8, 1}, {8, 0}, {7,0} ,{6,0} };
}

bool is_safe_square(int row,int col) {//    SAFEspace.coordinates = { {2,6},{6,12},{8,2},{12,8} };
    bool safe = 0;
    for (int i = 0; i < 4; i++) { 
        if (board[row][col]==board[SAFEspace.coordinates[i].first][SAFEspace.coordinates[i].second]) {
            safe++;
        }
    } 
    return safe;
}

void sendtokentoyard(playerslist* state, const string& player_name, char token_name) {
    pthread_mutex_lock(&state->mutex); // Lock the mutex to protect game state

    player* currentPlayer = state->players;
    bool playerFound = false;

    // Locate the player by name
    while (currentPlayer) {
        if (currentPlayer->name == player_name) {
            playerFound = true;
            break;
        }
        currentPlayer = currentPlayer->next;
    }

    if (!playerFound) {
        pthread_mutex_unlock(&state->mutex);
        cout << "Player not found: " << player_name << endl;
        return;
    }

    // Locate the token by name
    token* currentToken = currentPlayer->tokens;
    bool tokenFound = false;

    while (currentToken) {
        if (currentToken->name == token_name) {
            tokenFound = true;
            break;
        }
        currentToken = currentToken->next;
    }

    if (!tokenFound) {
        pthread_mutex_unlock(&state->mutex);
        cout << "Token not found for player " << player_name << ": " << token_name << endl;
        return;
    }

    // Define yard coordinates for each player
    vector<pair<int, int>> yardCoordinates;
    if (player_name == "blue") {

        yardCoordinates = { {5, 1}, {5, 2}, {5, 3},{5,4} }; // Example additional yard coordinates for player B
    }
    else if (player_name == "yellow") {
        yardCoordinates = { {5, 9}, {5, 10}, {5, 11},{5,12} }; // Example additional yard coordinates for player Y
    }
    else if (player_name == "red") {
        yardCoordinates = { {12, 5}, {11, 5}, {10, 5},{9,5} }; // Example additional yard coordinates for player R
    }
    else if (player_name == "green") {
        yardCoordinates = { {12, 12}, {11, 11}, {10, 10},{9,9} }; // Example additional yard coordinates for player G
    }
    else {
        pthread_mutex_unlock(&state->mutex);
        cout << "Invalid player name: " << player_name << endl;
        return;
    }

    // Check for available yard coordinates
    bool placedInYard = false;
    for (auto& coord : yardCoordinates) {
        int target_r = coord.first;
        int target_c = coord.second;

        // Check if the target yard coordinates are already occupied by another token
        bool coordinatesOccupied = false;
        player* tempPlayer = state->players;
        while (tempPlayer) {
            token* tempToken = tempPlayer->tokens;
            while (tempToken) {
                if (tempToken->r == target_r && tempToken->c == target_c) {
                    coordinatesOccupied = true;
                    break;
                }
                tempToken = tempToken->next;
            }
            if (coordinatesOccupied) break;
            tempPlayer = tempPlayer->next;
        }

        if (!coordinatesOccupied) {
            // If the coordinate is available, place the token there
            currentToken->r = target_r;
            currentToken->c = target_c;
            currentToken->home = true; // Set token back to yard/home
            currentToken->won = false;
            cout << "Player " << player_name << "'s Token " << token_name
                << " has been sent back to the yard at (" << currentToken->r << ", " << currentToken->c << ")." << endl;
            placedInYard = true;
            break;
        }
    }

    // If no available yard space was found
    if (!placedInYard) {
        pthread_mutex_unlock(&state->mutex);
        cout << "No available yard space for token " << token_name << " of player " << player_name << "." << endl;
        return;
    }

    pthread_mutex_unlock(&state->mutex); // Unlock the mutex
}


void handle_collisions(player* currentPlayer) {
    playerslist* state = manager.getPlayersList();
    pthread_mutex_lock(&state->mutex); // Lock the mutex to protect game state

    
    // Iterate over all players
    while (currentPlayer) {
        token* currentToken = currentPlayer->tokens;

        // Iterate over all tokens for the current player
        while (currentToken) {
            if (currentToken->home == true|| currentToken->won == true) {
                currentToken = currentToken->next;
                continue; // Skip tokens not in play (i.e., at home)
            }

            int current_pos_r = currentToken->r;
            int current_pos_c = currentToken->c;

            // Skip if on a safe square
            if (is_safe_square(current_pos_r, current_pos_c)) {
                currentToken = currentToken->next;
                continue;
            }

            // Check for collisions with other players' tokens
            player* opponentPlayer = state->players;
            while (opponentPlayer) {
                if (opponentPlayer == currentPlayer) {
                    opponentPlayer = opponentPlayer->next;
                    continue; // Skip the current player (same player)
                }

                token* opponentToken = opponentPlayer->tokens;
                while (opponentToken) {
                    if (opponentToken->home == true|| opponentToken->won == true) {
                        opponentToken = opponentToken->next;
                        continue; // Skip tokens not in play
                    }

                    // Check if the opponent's token is at the same position
                    if (opponentToken->r == current_pos_r && opponentToken->c == current_pos_c) {
                        // Collision detected
                        sendtokentoyard(state, opponentPlayer->name, opponentToken->name); // Send opponent's token to yard
                        // Update hit records
                        currentPlayer->hitcount++;
                        cout << "Player " << currentPlayer->name << " hit Player "
                            << opponentPlayer->name << "'s Token " << opponentToken->name
                            << ". Total Hits: " << currentPlayer->hitcount << endl;
                    }

                    opponentToken = opponentToken->next;
                }

                opponentPlayer = opponentPlayer->next;
            }

            currentToken = currentToken->next;
        }

        currentPlayer = currentPlayer->next;
    }

    pthread_mutex_unlock(&state->mutex); // Unlock the mutex
}
void handle_blocks(player* currentPlayer) {
    playerslist* state = manager.getPlayersList();
    pthread_mutex_lock(&state->mutex); // Lock the mutex to protect game state


    // Iterate over all players
    while (currentPlayer) {
        token* currentToken = currentPlayer->tokens;

        // Iterate over all tokens of the current player
        while (currentToken) {
            if (currentToken->home == true || currentToken->won == true) {
                currentToken = currentToken->next;
                continue; // Skip tokens not in play (i.e., at home)
            }

            int current_pos_r = currentToken->r;
            int current_pos_c = currentToken->c;

            // Check for other tokens of the same player on the same position
            int block_count = 1; // Current token counts as 1

            token* otherToken = currentPlayer->tokens;
            while (otherToken) {
                if (otherToken == currentToken) {
                    otherToken = otherToken->next;
                    continue; // Skip the current token itself
                }

                if (otherToken->home == true|| otherToken->won == true) {
                    otherToken = otherToken->next;
                    continue; // Skip tokens not in play
                }

                if (otherToken->r == current_pos_r && otherToken->c == current_pos_c) {
                    block_count++;
                }

                otherToken = otherToken->next;
            }

            // If two or more tokens are in the same position, a block is formed
            if (block_count >= 2) {
                cout << "Player " << currentPlayer->name << " has formed a block at position ("
                    << current_pos_r << ", " << current_pos_c << ") with "
                    << block_count << " tokens." << endl;

                // Implement block-specific logic if necessary
                // For example, prevent other players from landing on this block
            }

            currentToken = currentToken->next;
        }

        currentPlayer = currentPlayer->next;
    }

    pthread_mutex_unlock(&state->mutex); // Unlock the mutex
}

player* createPlayer(string name, string teammate, int numTokens, bool bot);

void Initializeplayers() {
    cout << "Enter number of players (2-4): ";
    cin >> numPlayers;
    while (numPlayers < 2 || numPlayers > 4) {
        cout << "Invalid input. Enter again: ";
        cin >> numPlayers;
    }

     numTokens;
    cout << "Enter number of tokens (1-4): ";
    cin >> numTokens;
    while (numTokens < 1 || numTokens > 4) {
        cout << "Invalid input. Enter again: ";
        cin >> numTokens;
    }
    string pstyle="\0";
    /*if (numPlayers == 4)
    {
        cout << "Input 'T' for Teamplayer or 'S' for Single player?" << endl;
        cin >> pstyle;
        while (pstyle != "t" && pstyle != "T" && pstyle != "s" && pstyle != "S")
        {
            cout << "Invalid Input. Try again" << endl;
            cin >> pstyle;
        }
    }*/
    char pbot='n';
    int pbnum = 0;
    /*cout << "Do you want any of the players to be bots? (Y/N)" << endl;
    cin >> pbot;
    while (pbot != 'Y' && pbot != 'y' && pbot != 'n' && pbot != 'N')
    {
        cout << "Invalid Input. Try again" << endl;
        cin >> pbot;
    }*/
    if (pbot == 'y' || pbot == 'Y')
    {
        cout << "How many players do you want to be bots?" << endl;
        cin >> pbnum;
        while (pbnum >= numPlayers || pbnum < (numPlayers - (numPlayers - 1)))
        {
            cout << "Invalid Input. Try again" << endl;
            cin >> pbnum;
        }

    }

    string names[] = { "blue", "yellow", "red", "green" };
    int botvar = 0;
    for (int i = 0; i < numPlayers; i++) {
        bool isbot = (botvar < pbnum);
        if (isbot)
            botvar++;
        //blue green, red yellow
        //
        string playstyle = (pstyle == "T" || pstyle == "t")? (i >0&&i<3) ? (i == 1) ? "red" :  "yellow" : (i == 0) ? "green" : "blue": "\0";
        player* newplayer = createPlayer(names[i], playstyle, numTokens, isbot);
        
        manager.addPlayer(newplayer);
    }
    


    startingtokenposition(numTokens);
    manager.displayPlayers();
}


// Create Board
void createboard() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (i < 6 && j < 6) {
                SetConsoleTextAttribute(hConsole, BLUE); // Blue area
                board[i][j] = 'B';
            }
            else if ((i < 6) && (j > 8 && j < 15)) {
                SetConsoleTextAttribute(hConsole, YELLOW); // Yellow area
                board[i][j] = 'Y';
            }
            else if ((i > 8 && i < 15) && (j < 6)) {
                SetConsoleTextAttribute(hConsole, RED); // Red area
                board[i][j] = 'R';
            }
            else if ((i > 8 && i < 15) && (j > 8 && j < 15)) {
                SetConsoleTextAttribute(hConsole, GREEN); // Green area
                board[i][j] = 'G';
            }
            else if ((i >= 6 && i <= 8) && (j >= 6 && j <= 8)) {
                SetConsoleTextAttribute(hConsole, WHITE); // White area
                board[i][j] = 'W';
            }
            else if (i == 7 && (j > 0 && j < 6)) {
                SetConsoleTextAttribute(hConsole, BLUE); // Blue area for the row
                board[i][j] = 'B';
            }
            else if (i == 7 && (j > 8 && j < 14)) {
                SetConsoleTextAttribute(hConsole, GREEN); // Green area for the row
                board[i][j] = 'G';
            }
            else if (j == 7 && (i > 0 && i < 6)) {
                SetConsoleTextAttribute(hConsole, YELLOW); // Yellow area for the column
                board[i][j] = 'Y';
            }
            else if (j == 7 && (i > 8 && i < 14)) {
                SetConsoleTextAttribute(hConsole, RED); // Red area for the column
                board[i][j] = 'R';
            }
            else {
                SetConsoleTextAttribute(hConsole, WHITE); // Default color for other spaces
                board[i][j] = '*';
            }
        }
        SetConsoleTextAttribute(hConsole, WHITE);
    }
    // Set specific player positions
    SetConsoleTextAttribute(hConsole, BLUE);
    board[6][1] = 'b';
    SetConsoleTextAttribute(hConsole, YELLOW);
    board[1][8] = 'y';
    SetConsoleTextAttribute(hConsole, RED);
    board[13][6] = 'r';
    SetConsoleTextAttribute(hConsole, GREEN);
    board[8][13] = 'g';
    safecoords.coordinates = { {6,1},{1,8},{13,6},{8,13} };//byrg
    SetConsoleTextAttribute(hConsole, WHITE);
    board[2][6] = 'S';
    board[6][12] = 'S';
    board[8][2] = 'S';
    board[12][8] = 'S';
    SetConsoleTextAttribute(hConsole, WHITE);
    SAFEspace.coordinates = { {2,6},{6,12},{8,2},{12,8} };
}
int getPlayerColor(string name);
// Token and Player Creation
token* createTokenList(int size,int color) {
    char names[] = { '1', '2', '3', '4' };
    token* head = nullptr;
    token* tail = nullptr;

    for (int i = 0; i < size; i++) {
        token* newToken = new token();
        newToken->name = names[i];
        newToken->color = color; // Assign the player's color to the token
        if (!head)
            head = tail = newToken;
        else {
            tail->next = newToken;
            tail = newToken;
        }
    }
    return head;
}

void displayTokenList(token* tokenList)
{
    while (tokenList)
    {
        cout << "name: " << tokenList->name << " | location: ";
        if (tokenList->won == 0)
            (tokenList->home == 0) ? cout << "Not at home\n" : cout << "at home\n";
        else
            cout << "won\n";
        tokenList = tokenList->next;
    }

}
int getPlayerColor(string name) {
    if (name == "blue") return 1; // Color code for blue
    if (name == "green") return 2; // Color code for green
    if (name == "red") return 4; // Color code for red
    if (name == "yellow") return 14; // Color code for yellow
    return 15; // Default color (white)
}
player* createPlayer(string name, string teammate, int numTokens, bool bot) {
    player* newPlayer = new player();
    newPlayer->name = name;
    newPlayer->teammate = teammate;
    newPlayer->tokens = createTokenList(numTokens, getPlayerColor(name));
    newPlayer->bot = bot;
    newPlayer->color = getPlayerColor(name); // Set color based on name
    //displayTokenList(newPlayer->tokens);
    return newPlayer;
}

void startingtokenposition(int numTokens) {
    playerslist* list = manager.getPlayersList();

    while (list) {
        player* current = list->players;
        while (current) {
            token* ctokens = current->tokens;
            string playerName = current->name;
            int row = (playerName == "blue"||playerName=="yellow") ? 1 : 9;
            int col = (playerName == "blue" || playerName == "red") ? 1 : 9;

            while (ctokens) {
                ctokens->r = row;
                ctokens->c = col++;
                board[ctokens->r][ctokens->c] = ctokens->name;
                ctokens = ctokens->next;
            }
            current = current->next;
        }
        list = list->next;
    }
}



bool arealltokenshome(token* tokenn)
{
    int tnum = 0, thome = 0;
    while (tokenn)
    {
        tnum++;
        if (tokenn->home)
            thome++;
        tokenn = tokenn->next;
    }
    return thome == tnum;
}

bool checkplayerposition(player* mplayer) {
    token* tokenn = mplayer->tokens;
    int tnum = 0, thome = 0, twon = 0;

    while (tokenn) {
        tnum++;
        if (tokenn->home)
            thome++;
        else if (tokenn->won)
            twon++;
        tokenn = tokenn->next;
    }
    if (dicenum[0] == 6 && thome > 0)
        return true;
    else if (thome == tnum || tnum == twon || tnum == (thome + twon))
        return false;

    return true;
}


pair<int, int> generatenewtokencoords(token* tok, string colour,int moveval) {
    char tcolor = colour[0];
    int row = tok->r, col = tok->c;
    vector<pair<int, int>> path;

    if (tcolor == 'b') path = bluePath.coordinates;
    else if (tcolor == 'y') path = yellowPath.coordinates;
    else if (tcolor == 'r') path = redPath.coordinates;
    else if (tcolor == 'g') path = greenPath.coordinates;

    // Find the current position in the path
    int i;
    for (i = 0; i < path.size(); ++i) {
        if (path[i].first == row && path[i].second == col) {
            break; // Index found
        }
    }i += moveval;
    if (i == 1 + path.size() && moveval == 1)
    {
        tok->won = 1;
        tok->r = 6; tok->c = 6;
        board[tok->r][tok->c] = tok->name;
    }

    if (i >= path.size())
        return path.back();
    
      

        // Return the new coordinates after moving forward by all dice rolls
    return { path[i].first, path[i].second}; // Token doesn't move if it's already won
}

void placetokenatstart(string name,token* t)
{//safecoords.coordinates = { {6,1},{1,8},{13,6},{8,13} };//byrg
    if (name == "blue")
    {
        board[t->r][t->c] = 'B';
        t->r = safecoords.coordinates[0].first;
        t->c = safecoords.coordinates[0].second;
        board[t->r][t->c]=t->name;
    }
    else if (name == "yellow")
    {
        board[t->r][t->c] = 'Y';
        t->r = safecoords.coordinates[1].first;
        t->c = safecoords.coordinates[1].second;
        board[t->r][t->c] = t->name;

    }
    else if (name == "red")
    {
        board[t->r][t->c] = 'R';
        t->r = safecoords.coordinates[2].first;
        t->c = safecoords.coordinates[2].second;
        board[t->r][t->c] = t->name;

    }
    else if (name == "green")
    {
        board[t->r][t->c] = 'G';
        t->r = safecoords.coordinates[3].first;
        t->c = safecoords.coordinates[3].second;
        board[t->r][t->c] = t->name;

    }

}int getColorFromCharacter(char c) {
    c = toupper(c); // Convert character to uppercase if it's not already
    switch (c) {
    case 'B': return BLUE; // Blue
    case 'G': return GREEN; // Green
    case 'R': return RED; // Red
    case 'Y': return YELLOW; // Yellow
    case 'W': return WHITE; // White (default)
    default: return WHITE; // Default color if no match
    }
}

bool istokenatstart(string name, token* t)
{//safecoords.coordinates = { {6,1},{1,8},{13,6},{8,13} };//byrg
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int safeCoord = 0; safeCoord < 4;safeCoord++ ) {
        if (t->r == safecoords.coordinates[safeCoord].first && t->c == safecoords.coordinates[safeCoord].second) {
            SetConsoleTextAttribute(hConsole, getColorFromCharacter(name[safeCoord]));
            board[t->r][t->c] = name[0];
            return true;
        }
    }
    return false;
}

//SAFEspace.coordinates = { {2,6},{6,12},{8,2},{12,8} };
bool istokenatsafe(string name, token* t)
{
    for (const auto& safeCoord : SAFEspace.coordinates) {
        if (t->r == safeCoord.first && t->c == safeCoord.second) {
            return true;
        }
    }
    return false;
}



void movetoken(player* state) {
    token* listoftokens = state->tokens;
    char tname;

    pthread_mutex_lock(&board_mutex); // Lock the board resource

    if (!checkplayerposition(state)) {
        cout << "No valid tokens to move for player " << state->name << endl;
        pthread_mutex_unlock(&board_mutex);
        return;
    }

    // Count dice values greater than 6
   

    for (int move = 0; dicenum[move]!=0&&move<3; ++move) {
        token* selectedToken = nullptr;
    picktoken:
        listoftokens = state->tokens; // Reset token list pointer

        if (state->bot == 0) {
            
                cout << "Pick token to move (move " << move + 1 << "): ";
                cin >> tname;
            

            while (listoftokens) {
                if (tname == listoftokens->name) {
                    selectedToken = listoftokens;
                    break;
                }
                listoftokens = listoftokens->next;
            }

            if (!selectedToken || (selectedToken->home && dicenum[0] != 6)) {
                cout << "Invalid selection. Try again." << endl;
                goto picktoken;
            }
        }
        else if (state->bot == 1) {
            // AI logic for automated token selection
            token* temp = state->tokens;
            while (temp) {
                if (!temp->home || (temp->home && dicenum[0] == 6)) {
                    selectedToken = temp;
                    break;
                }
                temp = temp->next;
            }

            if (!selectedToken) {
                cout << "No valid token found for the bot to move." << endl;
                pthread_mutex_unlock(&board_mutex);
                return;
            }
        }

        int oldRow = selectedToken->r, oldCol = selectedToken->c;

        if (selectedToken->home && dicenum[0] == 6) {
            selectedToken->home = false;
            placetokenatstart(state->name, selectedToken);
           /* dicenum[0] = dicenum[1];
            dicenum[1] = dicenum[2];
            dicenum[2] = 0;*/
            //board[selectedToken->r][selectedToken->c] =currentPlayer->name[0];
           /* pair<int, int> newCoords = generatenewtokencoords(selectedToken, currentPlayer->name);
            selectedToken->r = newCoords.first;
            selectedToken->c = newCoords.second;
            board[oldRow][oldCol] = char(currentPlayer->name[0]);
            board[selectedToken->r][selectedToken->c] = selectedToken->name;*/
           /* handle_collisions(state);
            handle_blocks(state); */
            cout << "Player: " << state->name << " Token: " << selectedToken->name << " has moved out of home." << endl;
        }
        else {
            pair<int, int> newCoords = generatenewtokencoords(selectedToken, state->name,dicenum[move]);
            
            if (istokenatstart(state->name, selectedToken))
                cout << "";
            else if (istokenatsafe(state->name, selectedToken))
                board[oldRow][oldCol] = 'S';
            else
            board[oldRow][oldCol] = '*';
            selectedToken->r = newCoords.first;
            selectedToken->c = newCoords.second;
            
            // Set the color based on the token's color attribute
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
              SetConsoleTextAttribute(hConsole, selectedToken->color);
            board[selectedToken->r][selectedToken->c] = selectedToken->name;
            SetConsoleTextAttribute(hConsole, 15);
            /* handle_collisions(state);
            handle_blocks(state);*/
            cout << "Token " << selectedToken->name << " moved to (" << newCoords.first << ", " << newCoords.second << ")." << endl;
        }

        displayboard();
    }

    pthread_mutex_unlock(&board_mutex);
}



bool checktokenposition(string name, int hitcount, token* listoftokens) {
    vector<pair<int, int>> blueramp = { {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5} };
    vector<pair<int, int>> yellowramp = { {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7} };
    vector<pair<int, int>> redramp = { {9, 7}, {10, 7}, {11, 7}, {12, 7}, {13, 7} };
    vector<pair<int, int>> greenramp = { {7, 9}, {7, 10}, {7, 11}, {7, 12}, {7, 13} };

    map<char, vector<pair<int, int>>> ramps = {
        {'b', blueramp}, {'y', yellowramp}, {'r', redramp}, {'g', greenramp}
    };

    if (hitcount == 0) {
        cout << "You need to kill at least one token to proceed home." << endl;
        return false;
    }

    char n = tolower(name[0]);
    if (ramps.find(n) != ramps.end()) {
        const auto& ramp = ramps[n];
        for (int i = 0, j = 5; i < ramp.size() && j > 0; ++i, --j) {
            if (listoftokens->r == ramp[i].first && listoftokens->c == ramp[i].second) {
                for(int k=0;k<3;k++)
                {
                    if (dicenum[k] > j) {
                        cout << "Dice number too big to move the token." << endl;
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


void initializeResources() {
    pthread_mutex_init(&dice_mutex, NULL);
    pthread_mutex_init(&board_mutex, NULL);
}

// Clean up resources
void cleanupResources() {
    pthread_mutex_destroy(&dice_mutex);
    pthread_mutex_destroy(&board_mutex);
}


void deleteTokenList(token* head) {
    while (head) {
        token* temp = head;
        head = head->next;
        delete temp;
    }
}

void deletePlayer(player* head) {
    while (head) {
        deleteTokenList(head->tokens);
        player* temp = head;
        head = head->next;
        delete temp;
    }
}

void deletePlayersList(playerslist* list) {
    while (list) {
        deletePlayer(list->players);
        playerslist* temp = list;
        list = list->next;
        delete temp;
    }
}

// Main Function
int main() {
    srand(time(0)); // Seed for randomness
    initializeResources();
    createboard();
    displayboard();
    initializePaths();
    Initializeplayers();
    displayboard();
   

    pthread_t* players = new pthread_t[numPlayers];
    pthread_t master;




    // Create player threads
    playerslist* currentList = manager.getPlayersList();
    
    player* currentPlayer = currentList->players;
    pthread_mutex_init(&currentList->mutex, NULL);//???
    pthread_cond_init(&currentList->cond_var, NULL);

    for (int i = 0; i < numPlayers; i++) {
        
        pthread_create(&players[i], NULL, player_thread, (void*)currentPlayer);
        currentPlayer = currentPlayer->next;
    }

    // Create master thread
    pthread_create(&master, NULL, master_thread, (void*)players);
    
    // Join threads
    for (int i = 0; i < numPlayers; i++) {
        pthread_join(players[i], NULL);
    }
    pthread_join(master, NULL);

    displayboard();


    delete[] players;
    deletePlayersList(manager.getPlayersList());
    pthread_mutex_destroy(&currentList->mutex);
    pthread_cond_destroy(&currentList->cond_var);

    cleanupResources();
    return 0;
}



