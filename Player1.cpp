#include "Player1.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "Catan.hpp"

namespace ariel
{

    Player::Player()
    {
        for (int i = 0; i < 6; ++i)
            resources_cards[i] = 0;
        for (int i = 0; i < 2; ++i)
            development_cards[i] = 0;
    }
    Player::Player(string p_name) : name(p_name), number(0)
    {
        for (int i = 0; i < 6; ++i)
            resources_cards[i] = 0;
        for (int i = 0; i < 2; ++i)
            development_cards[i] = 0;
        my_turn = 0;
        dice_rolled = 0;
        army = 0;
    }

    int Player::removeRandomNonZeroResourceCard()
    {
        std::vector<int> nonZeroIndices;
        for (int i = 0; i < 5; ++i)
        {
            if (resources_cards[i] > 0)
            { // Check for non-zero count
                nonZeroIndices.push_back(i);
            }
        }

        if (!nonZeroIndices.empty())
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, nonZeroIndices.size() - 1);
            int randomIndex = nonZeroIndices[dist(gen)];
            --resources_cards[randomIndex]; // Decrement the count of the selected resource card
            return randomIndex;
        }
        return -1; // Return -1 if no non-zero resource card is available
    }

    void Player::useKnight(Board board, string robbed, string hexa_landscape, int hexa_num, int seven)
    {
        // needs to check location - player

        if (development_cards[1] > 0)
        {
            development_cards[1]--;
            army++;
        }
        else if (!seven)
        {
            perror("dont use knight if you dont have one!");
            return;
        }

        for (int i = 0; i < 19; i++)
        {
            if (board.hexas[i].has_knight == 1)
            {
                board.hexas[i].has_knight = 0;
            }
            if ((board.hexas[i].landscape == hexa_landscape) &&
                (board.hexas[i].number == hexa_num))
            {
                board.hexas[i].has_knight = 1;
            }
        }
        int card = -1;
        Player p;
        if (next_turn->name == robbed)
        {
            p = *next_turn;
        }
        if (next_turn->next_turn->name == robbed)
        {
            p = *(next_turn->next_turn);
        }
        card = p.removeRandomNonZeroResourceCard();
        if (card != -1)
        {
            development_cards[card]++;
        }else{
        }
    }

    void Player::printCards()
    {
        cout << "The player's name is " << name << endl;
        cout << "The cards are:" << endl;
        string resources[6] = {"wood", "wool", "wheat", "ore", "bricks", "it is dessert!"};
        for (int i = 0; i < 5; i++)
        {
            cout << resources_cards[i] << " " << resources[i] << " cards" << endl;
        }
        for (int i = 0; i < 2; i++)
        {
            cout << development_cards[i] << " " << devCards[i] << " cards" << endl;
        }
    }

    void Player::buyDevelopmentCard()
    {
        vector<int> cards = {1, 2, 3};
        if (!charge(cards))
        {
            perror("dont buy if you dont have money!");
            return;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1); // Range: [1, 6]

        // Roll two dice
        int card_type = dist(gen);
        development_cards[card_type]++;
        cout << "you got dev card of:" << devCards[card_type] << endl;
    }

    void Player::rollDice(Board board)
    {
        if (!my_turn)
        {
            perror("You can't roll if it's not your turn!");
            return;
        }
        if (dice_rolled)
        {
            perror("You can't roll twice a turn!");
            return;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(1, 6);
        int die1 = dist(gen);
        int die2 = dist(gen);
        int sum = die1 + die2;
        if (sum == 7)
        {
            std::uniform_int_distribution<int> dist0(0, 18);
            int robbed_hexa = dist0(gen);
            string robbed_landscape = board.hexas[robbed_hexa].landscape;
            int robbed_number = board.hexas[robbed_hexa].number;
            std::uniform_int_distribution<int> dist1(0, 1);
            int next = dist1(gen);
            string robbed_name = next ? next_turn->name : next_turn->next_turn->name;
            useKnight(board, robbed_name, robbed_landscape, robbed_number, 1);
        }
        else
        {
            vector<vector<int>> cards = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
            for (int i = 0; i < 19; i++)
            {
                if (board.hexas[i].number == sum)
                {
                    if (board.hexas[i].has_knight)
                    {
                        continue;
                    }
                    for (int j = 0; j < 54; j++)
                    {
                        string nodes_name = board.nodes[j].name;
                        string hexas_letter = board.hexas[i].letter;
                        if (nodes_name.find(hexas_letter) != string::npos)
                        {
                            if (board.nodes[j].players_city > 0)
                            {
                                int player = board.nodes[j].players_city;
                                int is_city = player < 4 ? 1 : 2;
                                player = player < 4 ? player : player - 3;
                                cards[--player][(board.hexas[i].res_num)] = is_city;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 5; i++)
            {
                resources_cards[i] += cards[number - 1][i];
            }
            for (int i = 0; i < 5; i++)
            {
                next_turn->resources_cards[i] += cards[next_turn->number - 1][i];
            }
            for (int i = 0; i < 5; i++)
            {
                next_turn->next_turn->resources_cards[i] += cards[next_turn->next_turn->number - 1][i];
            }
        }
        dice_rolled = 1;
    }

    void Player::endTrurn()
    {
        if (!my_turn)
        {
            perror("You can't end what's not your's!");
            return;
        }
        my_turn = 0;
        dice_rolled = 0;
        next_turn->my_turn = 1;
    }

    string Player::find_node(vector<string> places, vector<int> placesNum, Board board)
    {
        string nodeName;
        int amount_of_places = places.size();
        for (int i = 0; i < amount_of_places; i++)
        {
            for (int j = 0; j < 19; j++)
            {
                if (board.hexas[j].landscape == places[i] &&
                    board.hexas[j].number == placesNum[i])
                {
                    nodeName += board.hexas[j].letter;
                }
            }
        }
        sort(nodeName.begin(), nodeName.end());
        return nodeName;
    }

    int Player::charge(vector<int> cards)
    {
        int amount_of_cards = cards.size();
        vector<int> resources_needed(6, 0);
        for (int i = 0; i < amount_of_cards; i++)
        {
            resources_needed[cards[i]]++;
        }
        for (int i = 0; i < 6; i++)
        {
            if (resources_needed[i] > resources_cards[i])
            {
                perror("dont buy if you dont have money!");
                return 0;
            }
        }
        for (int i = 0; i < amount_of_cards; i++)
        {
            --resources_cards[cards[i]];
        }

        return 1;
    }

    void Player::placeSettelemnt(vector<string> places, vector<int> placesNum, Board board)
    {
        if (!my_turn)
        {
            perror("dont buy if it's not your turn!");
            return;
        }
        string nodeName = find_node(places, placesNum, board);
        int has_road = 0;
        vector<string> neighbors;
        for (int i = 0; i < 144; i++)
        {
            if ((board.edges[i].first == nodeName) &&
                board.edges[i].players_road == number)
            {
                has_road = 1;
                neighbors.push_back(board.edges[i].second);
            }
        }

        if (!has_road)
        {
            perror("You can't place a settelemnt if you don't have road to here!");
            return;
        }

        int has_neighbor = 0;
        int size = neighbors.size();
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < 54; j++)
            {
                if ((board.nodes[j].name == neighbors[i]) && (board.nodes[j].players_city != 0))
                {
                    has_neighbor = 1;
                    break;
                }
            }
        }
        if (has_neighbor)
        {
            perror("You can't place a settelemnt if you have neighbors!");
            return;
        }

        for (int i = 0; i < 54; i++)
        {
            if (board.nodes[i].name == nodeName)
            {
                if (board.nodes[i].players_city != 0)
                {
                    perror("Settelemnt place already taken!");
                    return;
                }
                vector<int> cards = {0, 1, 2, 4};
                if (!charge(cards))
                {
                    perror("dont buy if you dont have money!");
                    return;
                }
                board.nodes[i].players_city = number;
                break;
            }
        }
    }
    void Player::placeCity(vector<string> places, vector<int> placesNum, Board board)
    {
        if (!my_turn)
        {
            perror("dont buy if it's not your turn!");
            return;
        }
        string nodeName = find_node(places, placesNum, board);
        for (int i = 0; i < 54; i++)
        {
            if (board.nodes[i].name == nodeName)
            {
                if (board.nodes[i].players_city != number)
                {
                    perror("You can't place a city if you don't have settelemnt here!");
                    return;
                }
                vector<int> cards = {2, 2, 3, 3, 3};
                if (!charge(cards))
                {
                    perror("dont buy if you dont have money!");
                    return;
                }
                board.nodes[i].players_city = number + 3;
                break;
            }
        }
    }

    void Player::placeRoad(vector<string> places, vector<int> placesNum, Board board)
    {
        if (!my_turn)
        {
            perror("dont buy if it's not your turn!");
            return;
        }
        string hexasStr = find_node(places, placesNum, board);
        string nodes[2];
        int j = 0;
        for (int i = 0; i < 54; i++)
        {
            if (board.nodes[i].name.find(hexasStr) != std::string::npos)
            {
                nodes[j] = board.nodes[i].name;
                j++;
            }
        }
        for (int i = 0; i < 54; i++)
        {
            if ((board.edges[i].first == nodes[0]) &&
                (board.edges[i].second == nodes[1]))
            {
                if (board.edges[i].players_road != 0)
                {
                    perror("Road place already taken!");
                    return;
                }
                else
                {
                    board.edges[i].players_road = number;
                    vector<int> cards = {0, 4};
                    if (!charge(cards))
                    {
                        perror("dont buy if you dont have money!");
                        return;
                    }
                }
            }
            if ((board.edges[i].first == nodes[1]) &&
                (board.edges[i].second == nodes[0]))
            {
                if (board.edges[i].players_road != 0)
                {
                    perror("Road place already taken!");
                    return;
                }
                board.edges[i].players_road = number;
            }
        }
    }

} // namespace ariel
