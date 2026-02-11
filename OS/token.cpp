////22L-7883 Zarmeena Fatima
//#pragma once
//#include<iostream>
//#include <string>
//#include<cctype>
//using namespace std; 
//
//struct token
//{
//    char name;
//    bool home = 1;
//    bool won = 0;
//    int r;
//    int c;
//    token* next;
//};
//struct player
//{
//    token* tokens;
//       string name;
// int hitcount = 0;
//    int turns = 0;
//    bool bot ;
//    string teammate = "\0";
//    player* next;
//    
//};
//struct playerslist
//{
//    player* players;
//    playerslist* next;
//};
//void displayTokenList(token* tokenList)
//{
//    while (tokenList)
//    {
//        cout << "name: " << tokenList->name <<" | location: ";
//        if (tokenList->won == 0)
//            (tokenList->home == 0) ? cout << "Not at home\n" : cout << "at home\n";
//        else
//            cout << "won\n";
//        tokenList = tokenList->next;
//    }
//    
//}
//void displayPlayer(player* player) {
//    while (player)
//    {
//        cout << "Player: " << player->name << " | hitcount: " << player->hitcount << " | turns: " << player->turns << " | bot: ";
//        (player->bot == 0) ? cout << "not a bot\n" : cout << "bot\n";
//        if (player->teammate != "\0")
//            cout << "Teamate: " << player->teammate << endl;
//        cout << "Tokens:\n"; displayTokenList(player->tokens);
//        cout << endl;
//        player = player->next;
//    }
//}
//class PlayersListManager {
//private:
//    playerslist* playerList;
//    // Private constructor
//    PlayersListManager() : playerList(NULL) {}
//
//public:
//    // Delete copy constructor and assignment operator to prevent duplicates
//    PlayersListManager(const PlayersListManager&) = delete;
//    void operator=(const PlayersListManager&) = delete;
//
//    // Singleton instance
//    static PlayersListManager& getInstance() {
//        static PlayersListManager instance;
//        return instance;
//    }
//    // Get the players list
//    playerslist*& getPlayersList() {
//        if (!playerList) {
//            playerList = new playerslist();
//            playerList->players = NULL;
//            playerList->next = NULL;
//        }
//        return playerList;
//    }
//
//    // Add player to the list
//    void addPlayer(player* newPlayer) {
//        playerslist*& list = getPlayersList();
//        if (!list->players) {
//            list->players = newPlayer;
//        }
//        else {
//            player* temp = list->players;
//            while (temp->next)
//                temp = temp->next;
//            temp->next = newPlayer;
//        }
//    }
//
//    // Display players list
//    void displayPlayers() {
//        playerslist* list = getPlayersList();
//        while (list) {
//            cout << "Players List\n";
//            displayPlayer(list->players);
//            cout << endl;
//            list = list->next;
//        }
//    }
//
//};
//
//    PlayersListManager& manager = PlayersListManager::getInstance();
//
//
//
//
//
//
//
//token* createTokenList(int size)
//{
//    char names[] = { '1','2','3','4' };
//    token* head = NULL;
//    token* tail = NULL;
//
//
//    for (int i = 0; i < size; i++)
//    {
//        token* newtoken = new token();
//        newtoken->name = names[i];
//        if (!head)
//            head = tail = newtoken;
//        else
//        {
//            tail->next = newtoken;
//            tail = newtoken;
//        }
//    }
//    return head;
//}
//player* createPlayer(string name, string teammate, int numtokens, bool bot)
//{  
//    player* newplayer = new player();
//    newplayer->teammate = teammate;
//  
//    newplayer->name = name;
//    newplayer->bot = bot;
//    newplayer->tokens = createTokenList(numtokens);
//    newplayer->next = NULL;
//    return newplayer;
//}
////
////void addPlayer( playerslist*& list, player* newplayer)
////{
////    if (!list)
////    {
////        list = new playerslist();
////        list->players = NULL;
////        list->next = NULL;
////    }
////    if (!list->players)
////        list->players = newplayer;
////    else
////    {
////        player* temp = list->players;
////        while (temp->next)
////            temp = temp->next;
////        temp->next = newplayer;
////    }
////}
//
//
////void displayPlayersList(playerslist* playerslist)
////{
////    while (playerslist)
////    {
////        cout << "Players List\n";
////        displayPlayer(playerslist->players);
////        cout << endl;
////        playerslist = playerslist->next;
////    }
////}
//
//void startingtokenposition(int tnum)
//{
//    PlayersListManager& manager = PlayersListManager::getInstance();
//    playerslist* list = manager.getPlayersList();
//
//    while (list) {
//        player* current = list->players;
//        while (current) {
//            token* ctokens = current->tokens;
//            string n = current->name;
//            char name = n[0];
//            int c1 = 1;
//            int c2 = 9;
//            while (ctokens)
//            {
//                if (name == 'b')
//                {
//                    ctokens->r = 1;
//                    ctokens->c = c1;
//                    c1++;
//                }
//                else if (name == 'y')
//                {
//                    ctokens->r = 1;
//                    ctokens->c = c2;
//                    c2++;
//                }
//                else if (name == 'r')
//                {
//                    ctokens->r = 9;
//                    ctokens->c = c1;
//                    c1++;
//                }
//                else if (name == 'g')
//                {
//                    ctokens->r = 9;
//                    ctokens->c = c2;
//                    c2++;
//                }
//                if (c1 > 6)c1 = 1;
//                if (c2 > 15)c2 = 9;
//
//                ctokens = ctokens->next;
//            }
//
//
//            current = current->next;
//        }
//        list = list->next;
//    }
//  
//
//}
//void Initializeplayers()
//{
//    int pnum;//player number
//    string pstyle="\0";//player style (team or single)
//    char pbot;//player bots
//    int pbnum;//player bots number
//    int tnum;//token number
//
//    //number of players
//    cout << "How many players are playing? (2-4)" << endl;
//    cin >> pnum;
//    while (pnum < 2 || pnum>4)
//    {
//        cout << "Invalid number of players. Input again" << endl;
//        cin >> pnum;
//    }
//    //teammates or single player
//    if(pnum==4)
//    {
//        cout << "Input 'T' for Teamplayer or 'S' for Single player?" << endl;
//        cin >> pstyle;
//        while (pstyle != "t" && pstyle != "T" && pstyle != "s" && pstyle != "S")
//        {
//            cout << "Invalid Input. Try again" << endl;
//            cin >> pstyle;
//        }
//    }
//    //rest of player to be bots or not?
//    cout << "Do you want any of the players to be bots? (Y/N)" << endl;
//    cin >> pbot;
//    while (pbot != 'Y' && pbot != 'y' && pbot != 'n' && pbot != 'N')
//    {
//        cout << "Invalid Input. Try again" << endl;
//        cin >> pbot;
//    }
//    if(pbot=='y'||pbot=='Y')
//    {
//        cout << "How many players do you want to be bots?" << endl;
//        cin >> pbnum;
//        while (pbnum >= pnum || pbnum < (pnum - (pnum - 1)))
//        {
//            cout << "Invalid Input. Try again" << endl;
//            cin >> pbnum;
//        }
//
//    }
//   
//
//    //number of tokens to play game with
//    cout << "How many tokens do you want to play with? (1-4)" << endl;
//    cin >> tnum;
//    while (tnum < 1 || tnum>4)
//    {
//        cout << "Invalid number of tokens, Input again" << endl;
//        cin >> tnum;
//    }
//
//    //singleton use
//    PlayersListManager& manager = PlayersListManager::getInstance();
//    string names[4] = { "yellow", "green", "red", "blue" };
//    int botvar = 0;
//    for (int i = 1; i <= pnum; i++) {
//        bool isbot = (botvar < pbnum);
//        if (isbot)
//            botvar++;
//
//        string playstyle = (pstyle == "T" || pstyle == "t")
//            ? (i % 2 == 0) ? (i == 2) ? "blue" : "green" : (i == 1) ? "red" : "yellow"
//            : "\0";
//        player* newplayer = createPlayer(names[i - 1], playstyle, tnum, isbot);
//        manager.addPlayer(newplayer);
//    }
//    startingtokenposition(tnum);
//    manager.displayPlayers();
//
//}
//
//
//
//
//
//
//
//
//void updatehitcount(string name) {
//    PlayersListManager& manager = PlayersListManager::getInstance();
//    playerslist* list = manager.getPlayersList();
//
//    while (list) {
//        player* current = list->players;
//        while (current) {
//            if (current->name == name) {
//                current->hitcount++;
//                cout << "Hit count updated for player " << name << ". New hit count: " << current->hitcount << endl;
//                return;
//            }
//            current = current->next;
//        }
//        list = list->next;
//    }
//    cout << "Player with name " << name << " not found!" << endl;
//}
//
//void updateturns(string name) {
//    PlayersListManager& manager = PlayersListManager::getInstance();
//    playerslist* list = manager.getPlayersList();
//
//    while (list) {
//        player* current = list->players;
//        while (current) {
//            if (current->name == name) {
//            current->turns++;
//            cout << "Turn updated for player " << current->name << ". New turn count: " << current->turns << endl;
//            }
//            current = current->next;
//        }
//        list = list->next;
//    }
//}
//
//void updatelocation(string name) {
//    PlayersListManager& manager = PlayersListManager::getInstance();
//    playerslist* list = manager.getPlayersList();
//
//    while (list) {
//        player* current = list->players;
//        while (current) {
//            if (current->name == name) {
//                token* tokenList = current->tokens;
//                while (tokenList) {
//                    if (tokenList->home) {
//                        tokenList->home = false; // Simulate token moving out of home
//                        cout << "Token " << tokenList->name << " of player " << current->name << " moved out of home.\n";
//                    }
//                    else {
//                        tokenList->won = true; // Simulate token reaching the winning position
//                        cout << "Token " << tokenList->name << " of player " << current->name << " has won!\n";
//                    }
//                    tokenList = tokenList->next;
//                }
//            }
//            current = current->next;
//        }
//        list = list->next;
//    }
//}
//
//
//void deleteTokenList(token* head) {
//    while (head) {
//        token* temp = head;
//        head = head->next;
//        delete temp;
//    }
//}
//
//void deletePlayer(player* head) {
//    while (head) {
//        deleteTokenList(head->tokens);
//        player* temp = head;
//        head = head->next;
//        delete temp;
//    }
//}
//
//void deletePlayersList(playerslist* list) {
//    while (list) {
//        deletePlayer(list->players);
//        playerslist* temp = list;
//        list = list->next;
//        delete temp;
//    }
//}
//
//
//
