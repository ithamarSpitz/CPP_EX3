#include "Catan.hpp"
#include "Player.hpp"

namespace ariel
{

    Catan::Catan(){}

    Catan::Catan(Player &player1, Player &player2, Player &player3)
    {
        players[0] = &player1;
        players[1] = &player2;
        players[2] = &player3;
        board = createBoard();
    }

    void Catan::ChooseStartingPlayer()
    {
        int ps[3] = {1, 2, 3};
        random_device rd;
        mt19937 gen(rd());
        shuffle(ps, ps + 3, gen);

        // Assign shuffled players and set their numbers
        players[0]->number = ps[0];
        players[1]->number = ps[1];
        players[2]->number = ps[2];
        for (int i = 0; i < 3; ++i)
        {
            if (players[i]->number == 1)
            {
                swap(players[0], players[i]);
                break;
            }
        }
        for (int i = 1; i < 3; ++i)
        {
            if (players[i]->number == 2)
            {
                swap(players[1], players[i]);
                break;
            }
        }
        players[0]->my_turn = 1;
        players[0]->next_turn = players[1];
        players[1]->next_turn = players[2];
        players[2]->next_turn = players[0];
        for (int i = 0; i < 3; i++)
        {
            players[i]->resources_cards[0] = 4;
            players[i]->resources_cards[4] = 4;
            players[i]->resources_cards[1] = 2;
            players[i]->resources_cards[2] = 2;
        }
    }

    Board Catan::getBoard()
    {
        return board;
    }

    void Catan::printWinner()
    {
        int sums[3] = {0, 0, 0};
        int biggest_army = 0;
        int biggest_army_holder = -1;
        for (int i = 0; i < 3; i++)
        {
            if (biggest_army < players[i]->army)
            {
                biggest_army_holder = i;
            }
        }

        if (biggest_army_holder != -1)
        {
            sums[biggest_army_holder] += 2;
            cout << "biggest_army_holder is: " << players[biggest_army_holder]->name << endl;
        }

        int roades[3] = {0, 0, 0};
        for (int i = 0; i < 144; i++)
        {
            if (board.edges[i].players_road != 0)
            {
                roades[board.edges[i].players_road - 1]++;
            }
        }
        int longest_road = 0;
        int longest_road_holder = -1;
        for (int i = 0; i < 3; i++)
        {
            if (longest_road < roades[i])
            {
                longest_road_holder = i;
            }
        }
        if (longest_road_holder != -1)
        {
            sums[longest_road_holder] += 2;
            cout << "longest_road_holder is: " << players[longest_road_holder]->name << endl;
        }

        for (int i = 0; i < 54; i++)
        {
            int p = board.nodes[i].players_city;
            if (p != 0)
            {
                if (p < 4)
                {
                    sums[p - 1]++;
                }
                else
                {
                    p = p - 3;
                    sums[p] += 2;
                }
            }
        }

        for (int i = 0; i < 3; i++)
        {
            sums[i] += players[i]->development_cards[0];
        }

        int winner = -1;
        int sum = 0;
        for (int i = 0; i < 3; i++)
        {
            if (sum < sums[i])
            {
                sum = sums[i];
                winner = i;
            }
        }
        if (winner != -1)
        {
            cout << "the winner is: " << players[winner]->name << endl;
        }
    }

    Hexa Catan::new_hexa(Hexa hexa, int i, int type, int number)
    {
        char letter = 'a' + i;
        hexa.letter = std::string(1, letter);
        hexa.landscape = landscapes[type];
        hexa.resource = resources[type];
        hexa.number = number;
        hexa.res_num = type;
        if (number == 0)
        {
            hexa.has_knight = 1;
        }
        else
        {
            hexa.has_knight = 0;
        }
        return hexa;
    }

    Edge Catan::new_edge(Edge edge, string namei, string namej)
    {
        edge.first = namei;
        edge.second = namej;
        edge.players_road = 0;
        return edge;
    }

    Node *Catan::createNodes()
    {
        Node *nodes = new Node[54];
        // Populate nodes as needed
        for (int i = 0; i < 54; ++i)
        {
            nodes[i].name = names[i];
            nodes[i].players_city = 0;
        }
        return nodes;
    }

    Edge *Catan::createEdges()
    {
        Edge *edges = new Edge[144];
        int counter = 0;
        for (int i = 0; i < 54; i++)
        {
            for (int j = i + 1; j < 54; j++)
            {
                int matchingLetters = 0;
                if (names[i].length() == 3)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        if (names[j].find(names[i][k]) != string::npos)
                        {
                            matchingLetters++;
                        }
                    }
                    if (matchingLetters == 2)
                    {
                        edges[counter] = new_edge(edges[counter], names[i], names[j]);
                        counter++;
                        edges[counter] = new_edge(edges[counter], names[j], names[i]);
                        counter++;
                    }
                }
                if (names[i].length() == 2)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        if (names[j].find(names[i][k]) != string::npos)
                        {
                            matchingLetters++;
                        }
                    }
                    if (matchingLetters == 2)
                    {
                        edges[counter] = new_edge(edges[counter], names[i], names[j]);
                        counter++;
                        edges[counter] = new_edge(edges[counter], names[j], names[i]);
                        counter++;
                    }
                    if (matchingLetters == 1)
                    {
                        if (names[j].length() == 1)
                        {
                            edges[counter] = new_edge(edges[counter], names[i], names[j]);
                            counter++;
                            edges[counter] = new_edge(edges[counter], names[j], names[i]);
                            counter++;
                        }
                        if (names[j].length() == 2 &&
                            names[i][0] == names[j][0] &&
                            names[j][1] == '1')
                        {
                            edges[counter] = new_edge(edges[counter], names[i], names[j]);
                            counter++;
                            edges[counter] = new_edge(edges[counter], names[j], names[i]);
                            counter++;
                        }
                    }
                }
                if (names[i].length() == 1)
                {
                    if (names[j].length() == 2 &&
                        names[j].find(names[i][0]) != string::npos)
                    {
                        edges[counter] = new_edge(edges[counter], names[i], names[j]);
                        counter++;
                        edges[counter] = new_edge(edges[counter], names[j], names[i]);
                        counter++;
                    }
                }
            }
        }
        for (int i = 0; i < 12; i++)
        {
            edges[counter] = new_edge(edges[counter], additional_edges[i * 2], additional_edges[i * 2 + 1]);
            counter++;
            edges[counter] = new_edge(edges[counter], additional_edges[i * 2 + 1], additional_edges[i * 2]);
            counter++;
        }
        return edges;
    }

    Hexa *Catan::createHexas()
    {
        Hexa *hexas = new Hexa[19];
        int shuffle_landscape[19] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(shuffle_landscape, shuffle_landscape + 19, gen);

        int arr_shuffle_numbers[18] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
        std::shuffle(arr_shuffle_numbers, arr_shuffle_numbers + 18, gen);

        int number_index = 0;

        for (int i = 0; i < 19; ++i)
        {
            int type = shuffle_landscape[i];
            int number = (type == 5) ? 0 : arr_shuffle_numbers[number_index++];
            hexas[i] = new_hexa(hexas[i], i, type, number);
        }

        return hexas;
    }

    Board Catan::createBoard()
    {
        Board board;
        board.nodes = createNodes();
        board.edges = createEdges();
        board.hexas = createHexas();
        return board;
    }

} // namespace ariel
