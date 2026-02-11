////22L-7883 Zarmeena Fatima
//#pragma once
//#include<iostream>
////#include "token.cpp"
//
//using namespace std;
//class PlayersListManager;
//extern PlayersListManager& manager;
//extern struct token* tokenList;
//extern struct player* playerList;
//extern struct playerslist* listofplayers;
//
//char board[15][15];
//
//void displayboard()
//{
//	for (int i = 0; i < 15; i++)
//	{
//
//		for (int j = 0; j < 15; j++)
//		{
//			cout << board[i][j]<<"  ";
//		}
//		cout <<endl;
//
//	}
//}
//
//
////dice roll
//int rollDice() {
//	return rand() % 6 + 1;
//}
//void displayinitialposition()
//{
//    PlayersListManager& manager = PlayersListManager::getInstance();
//    playerslist* list = manager.getPlayersList();
//
//    while (list) {
//        player* current = list->players;
//        while (current) {
//            token* ctokens = current->tokens;
//            
//            while (ctokens)
//            {
//				board[ctokens->r][ctokens->c] = ctokens->name;
//
//                ctokens = ctokens->next;
//            }
//
//
//            current = current->next;
//        }
//        list = list->next;
//    }
//}
//
//
//void createboard()
//{
//for (int i = 0; i < 15; i++)
//{
//	for (int j = 0; j < 15; j++)
//	{
//		//home bases
//		if (i < 6 && j < 6)
//			board[i][j] = 'B';//Blue
//		else if ((i < 6) && (j > 8 && j < 15))
//			board[i][j] = 'Y';//Yellow
//		else if ((i > 8 && i < 15) && (j < 6))
//			board[i][j] = 'R';//Red
//		else if ((i > 8 && i < 15) && (j > 8 && j < 15))
//			board[i][j] = 'G';//Green
//		//winners area
//		else if ((i >= 6 && i <= 8) && (j >= 6 && j <= 8))
//			board[i][j] = 'W';
//		//colour ramps
//		else if (i == 7 && (j > 0 && j < 6))
//			board[i][j] = 'B';//Blue Ramp
//		else if (i == 7 && (j > 8 && j < 14))
//			board[i][j] = 'G';//Green Ramp
//		else if (j == 7 && (i > 0 && i < 6))
//			board[i][j] = 'Y';//Yellow Ramp
//		else if (j == 7 && (i > 8 && i < 14))
//			board[i][j] = 'R';//Red Ramp
//		else
//			board[i][j] = '*';
//
//	}
//}
////Starting Points
//board[6][1] = 'b';//Blue Start
//board[1][8] = 'y';//Yellow Start
//board[13][6] = 'r';//Red Start
//board[8][13] = 'g';//Green Start
////Safe Points
//board[2][6] = 'S';
//board[6][12] = 'S';
//board[8][2] = 'S';
//board[12][8] = 'S';
//
//displayinitialposition();
//}
