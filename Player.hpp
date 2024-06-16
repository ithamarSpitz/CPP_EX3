#ifndef PLAYER_H
#define PLAYER_H

#include "Catan.hpp"

using namespace std;
namespace ariel
{

    class Player
    {
    public:
        string devCards[2] = {"point", "knight"};
        string name;
        int number;
        int resources_cards[5];
        int development_cards[2];
        int my_turn;
        int dice_rolled;
        Player *next_turn;
        int army;

        Player();
        Player(string p_name);

        int removeRandomNonZeroResourceCard();
        void useKnight(Board &board, string robbed, string hexa_landscape, int hexa_num, int seven);
        void printCards();
        void buyDevelopmentCard();
        void rollDice(Board &board);
        void endTrurn();
        string find_node(vector<string> places, vector<int> placesNum, Board &board);
        int charge(vector<int> cards);
        void placeSettelemnt(vector<string> places, vector<int> placesNum, Board &board);
        void placeCity(vector<string> places, vector<int> placesNum, Board &board);
        void placeRoad(vector<string> places, vector<int> placesNum, Board &board);
    };

} // namespace ariel

#endif // PLAYER_H
