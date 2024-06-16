#ifndef CATAN_H
#define CATAN_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

namespace ariel
{

    struct Node
    {
        string name;
        int players_city;
    };

    struct Edge
    {
        Edge(){};
        string first;
        string second;
        int players_road;
    };

    struct Hexa
    {
        string letter;
        string resource;
        string landscape;
        int number;
        int res_num;
        int has_knight;
    };

    struct Board
    {
        Node *nodes;
        Edge *edges;
        Hexa *hexas;
    };

    class Player;

    class Catan
    {
    public:
        Board board;
        Player *players[3];

        Catan();
        Catan(Player &player1, Player &player2, Player &player3);
        void ChooseStartingPlayer();
        Board getBoard();
        void printWinner();
        Board createBoard();

    private:
        string landscapes[6] = {
            "forest", "pasture land", "agricaltural land",
            "mountains", "hills", "desert"};
        string resources[6] = {
            "wood", "wool", "wheat", "ore", "bricks", ""};
        // each node called by its hexagon letter. hexas with 2 single-lettered nodes hadded 0/1 clockwise
        string names[54] = {
            "a0", "a1", "ab", "ad", "ade", "abe",
            "b", "bc", "bcf", "bef",
            "c0", "c1", "cg", "cfg",
            "d", "dei", "dhi", "dh",
            "eij", "efj",
            "fjk", "fgk",
            "g", "gl", "glk",
            "h0", "h1", "him", "hm",
            "ijn", "imn",
            "jko", "jno",
            "klp", "kop",
            "l0", "l1", "lp",
            "m", "mnq", "mq",
            "nor", "nqr",
            "ops", "ors",
            "p", "ps",
            "q0", "q1", "qr",
            "r", "rs",
            "s0", "s1"};
        string additional_edges[24] = {
            "a0", "ad",
            "a1", "ab",
            "c0", "bc",
            "c1", "cg",
            "h0", "hm",
            "h1", "dh",
            "l0", "gl",
            "l1", "lp",
            "q0", "qr",
            "q1", "mq",
            "s0", "ps",
            "s1", "rs"};

        Hexa new_hexa(Hexa hexa, int i, int type, int number);
        Edge new_edge(Edge edge, string namei, string namej);
        Node *createNodes();
        Edge *createEdges();
        Hexa *createHexas();
    };

} // namespace ariel

#endif // CATAN_H
