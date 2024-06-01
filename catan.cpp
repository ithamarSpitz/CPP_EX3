#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib> 
#include <algorithm>
#include <random> 
using namespace std;

struct Node {
    string name;
    int players_city;
};
struct Edge {
    string first;
    string second;
    int players_road;
};
struct Hexa {
    string letter;
    string resource;
    string landscape;
    int number;
    int res_num;
    int has_knight;
};

struct Board{
    Node* nodes;
    Edge* edges; 
    Hexa* hexas;
};

struct Player {
    string devCards[2] = {"point","knight"};
    string name;
    int number;
    int resources_cards[5];
    int development_cards[2];
    int my_turn;
    int dice_rolled;
    Player *next_turn;
    int army;

    Player() {
        for (int i = 0; i < 6; ++i) resources_cards[i] = 0;
        for (int i = 0; i < 2; ++i) development_cards[i] = 0;
    }
    Player(string p_name):
     name(p_name), number(0){
        for (int i = 0; i < 6; ++i) resources_cards[i] = 0;
        for (int i = 0; i < 2; ++i) development_cards[i] = 0;
        my_turn = 0;
        dice_rolled = 0;
        army = 0;
    }

    int removeRandomNonZeroResourceCard() {
        std::vector<int> nonZeroIndices;
        for (int i = 0; i < 5; ++i) {
            if (resources_cards[i] > 0) { // Check for non-zero count
                nonZeroIndices.push_back(i);
            }
        }

        if (!nonZeroIndices.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, nonZeroIndices.size() - 1);
            int randomIndex = nonZeroIndices[dist(gen)];
            resources_cards[randomIndex]--; // Decrement the count of the selected resource card
            return randomIndex;
        }
        return -1; // Return -1 if no non-zero resource card is available
    }

    void useKnight(Board board, string robbed, string hexa_landscape, int hexa_num, int seven){
        // needs to check location - player
        
        if(development_cards[1] > 0){
            development_cards[1]--;
            army++;
        }else if (!seven){
            perror("dont use knight if you dont have one!");
            return;
        }

        for (int i = 0; i < 19; i++){
            if(board.hexas[i].has_knight == 1){
                board.hexas[i].has_knight = 0;
            }
            if((board.hexas[i].landscape == hexa_landscape)&&
            (board.hexas[i].number == hexa_num)){
                board.hexas[i].has_knight = 1;
            }
        }
        int card = -1;
        Player p;
        if(next_turn->name == robbed){
            p = *next_turn;
        }
        if(next_turn->next_turn->name == robbed){
            p = *(next_turn->next_turn);
        }
        //card = p.removeRandomNonZeroResourceCard();
        if (card != -1){
            development_cards[card]++;
        }
    }

    void printCards(){
        cout << "The player's name is " << name << endl;
        cout << "The cards are:" << endl;
        string resources[6] = {"wood", "wool", "wheat", "ore", "bricks", "it is dessert!"};
        for (int i = 0; i < 5; i++){
            cout << resources_cards[i] << " " << resources[i] << " cards"<< endl;
        }        
        for (int i = 0; i < 2; i++){
            cout << development_cards[i] << " " << devCards[i] << " cards"<< endl;
        }
        
    }

    void buyDevelopmentCard(){
        vector<int> cards = {1,2,3};
        if(!charge(cards)){
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
    
    void rollDice(Board board){
        if(!my_turn){
            perror("You can't roll if it's not your turn!");
            return;
        }        
        if(dice_rolled){
            perror("You can't roll twice a turn!");
            return;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(1, 6);
        int die1 = dist(gen);
        int die2 = dist(gen);
        int sum = die1 + die2;
        if (sum == 7){
            std::uniform_int_distribution<int> dist0(0, 18);
            int robbed_hexa = dist0(gen);
            string robbed_landscape = board.hexas[robbed_hexa].landscape;                  
            int robbed_number = board.hexas[robbed_hexa].number;                  
            std::uniform_int_distribution<int> dist1(0, 1);
            int next = dist1(gen);
            string robbed_name = next? next_turn->name : next_turn->next_turn->name;
            useKnight(board, robbed_name, robbed_landscape, robbed_number, 1);
        }else{
            vector<vector<int>> cards = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
            for (int i = 0; i < 19; i++){
                if(board.hexas[i].number == sum){
                    if (board.hexas[i].has_knight){
                        continue;
                    }
                    for (int j = 0; j < 54; j++){
                        string nodes_name = board.nodes[j].name;
                        string hexas_letter = board.hexas[i].letter;
                        if(nodes_name.find(hexas_letter) != string::npos){
                            if(board.nodes[j].players_city > 0){
                                int player = board.nodes[j].players_city;
                                int is_city = player<4? 1:2;
                                player = player<4? player : player-3;
                                cards[--player][ (board.hexas[i].res_num) ] = is_city;
                            }
                        }
                    }
                }
            }
            

            for (int i = 0; i < 5; i++){
                resources_cards[i] += cards[number-1][i];
            }
            for (int i = 0; i < 5; i++){
                next_turn->resources_cards[i] += cards[next_turn->number-1][i];
            }
            for (int i = 0; i < 5; i++){
                next_turn->next_turn->resources_cards[i] += cards[next_turn->next_turn->number-1][i];
            }
        }
        dice_rolled = 1;
    }

    void endTrurn(){
        if(!my_turn){
            perror("You can't end what's not your's!");
            return;
        }  
        my_turn = 0;
        dice_rolled = 0;
        next_turn->my_turn = 1;
    }
    
    string find_node(vector<string> places, vector<int> placesNum, Board board){
        string nodeName;
        int amount_of_places = places.size();
        for (int i = 0; i < amount_of_places; i++){
            for (int j = 0; j < 19; j++){
                if(board.hexas[j].landscape == places[i] &&
                board.hexas[j].number == placesNum [i]){
                    nodeName += board.hexas[j].letter;
                }
            }
        }
        sort(nodeName.begin(), nodeName.end());
        return nodeName;
    }

    int charge(vector<int> cards){
        int amount_of_cards = cards.size();
        vector<int> resources_needed(6, 0);
        for (int i = 0; i < amount_of_cards; i++){
            resources_needed[cards[i]]++;
        }
        for (int i = 0; i < 6; i++){
            if (resources_needed[i] > resources_cards[i]){
                perror("dont buy if you dont have money!");
                return 0;
            }
        }
        for (int i = 0; i < amount_of_cards; i++){
            --resources_cards[cards[i]];
        }
        
        return 1;
    }
    
    void placeSettelemnt(vector<string> places, vector<int> placesNum, Board board){
        if(!my_turn){
            perror("dont buy if it's not your turn!");
            return;
        }
        string nodeName = find_node(places, placesNum, board);
        int has_road = 0;
        vector<string> neighbors;
        for (int i = 0; i < 144; i++){
            if((board.edges[i].first == nodeName)&&
            board.edges[i].players_road == number){
                has_road = 1;
                neighbors.push_back(board.edges[i].second);
            }
        }
        
        if(!has_road){
            perror("You can't place a settelemnt if you don't have road to here!");
            return;
        }

        int has_neighbor = 0;
        int size = neighbors.size();
        for (int i = 0; i < size; i++){
            for (int j = 0; j < 54; j++){
                if((board.nodes[j].name == neighbors[i])
                &&(board.nodes[j].players_city != 0)){
                    has_neighbor = 1;
                    break;
                }
            }
        }
        if(has_neighbor){
            perror("You can't place a settelemnt if you have neighbors!");
            return;
        }        

        for (int i = 0; i < 54; i++){
            if(board.nodes[i].name == nodeName){
                if(board.nodes[i].players_city != 0){
                    perror("Settelemnt place already taken!");
                    return;
                }
                vector<int> cards = {0, 1, 2, 4};
                if(!charge(cards)){
                    perror("dont buy if you dont have money!");
                    return;
                }
                board.nodes[i].players_city = number;
                break;
            }
        }
    }
    void placeCity(vector<string> places, vector<int> placesNum, Board board){
        if(!my_turn){
            perror("dont buy if it's not your turn!");
            return;
        }        
        string nodeName = find_node(places, placesNum, board);
        for (int i = 0; i < 54; i++){
            if(board.nodes[i].name == nodeName){
                if(board.nodes[i].players_city != number){
                    perror("You can't place a city if you don't have settelemnt here!");
                    return;
                }
                vector<int> cards = {2, 2, 3, 3, 3};
                if(!charge(cards)){
                    perror("dont buy if you dont have money!");
                    return;
                }
                board.nodes[i].players_city = number+3;
                break;
            }
        }
    }

    void placeRoad(vector<string> places, vector<int> placesNum, Board board){
        if(!my_turn){
            perror("dont buy if it's not your turn!");
            return;
        }
        string hexasStr = find_node(places, placesNum, board);
        string nodes[2];
        int j = 0; 
        for (int i = 0; i < 54; i++){
            if (board.nodes[i].name.find(hexasStr) != std::string::npos) {
                nodes[j] = board.nodes[i].name;
                j++;
            }
        }
        for (int i = 0; i < 54; i++){
            if((board.edges[i].first == nodes[0])&&
            (board.edges[i].second == nodes[1])){
                if(board.edges[i].players_road != 0){
                    perror("Road place already taken!");
                    return;
                }else{
                    board.edges[i].players_road = number;
                    vector<int> cards = {0,4};
                    if(!charge(cards)){
                        perror("dont buy if you dont have money!");
                        return;
                    }
                }
            }
            if((board.edges[i].first == nodes[1])&&
            (board.edges[i].second == nodes[0])){
                if(board.edges[i].players_road != 0){
                    perror("Road place already taken!");
                    return;
                }
                board.edges[i].players_road = number;
            }
        }
    
    }
};

struct Catan{
    Board board;
    Player *players[3];
    Catan(Player &player1, Player &player2, Player &player3) {
        players[0] = &player1;
        players[1] = &player2;
        players[2] = &player3;
        board = createBoard();
    }
    void ChooseStartingPlayer(){
        int ps[3] = {1, 2, 3};
        random_device rd;
        mt19937 gen(rd());
        shuffle(ps, ps + 3, gen);

        // Assign shuffled players and set their numbers
        players[0]->number = ps[0];
        players[1]->number = ps[1];
        players[2]->number = ps[2];
        for (int i = 0; i < 3; ++i) {
            if (players[i]->number == 1) {
                swap(players[0], players[i]);
                break;
            }
        }
        for (int i = 1; i < 3; ++i) {
            if (players[i]->number == 2) {
                swap(players[1], players[i]);
                break;
            }
        }
        players[0]-> my_turn = 1;
        players[0]->next_turn = players[1];
        players[1]->next_turn = players[2];
        players[2]->next_turn = players[0];
        for (int i = 0; i < 3; i++){
            players[i]->resources_cards[0] = 4;
            players[i]->resources_cards[4] = 4;
            players[i]->resources_cards[1] = 2;
            players[i]->resources_cards[2] = 2;
        }
    }
    Board getBoard() {return board;}
    void printWinner() {
        int sums[3] = {0,0,0};
        int biggest_army = 0;
        int biggest_army_holder = -1;        
        for (int i = 0; i < 3; i++){
            if(biggest_army < players[i]->army){
                biggest_army_holder = i;
            }
        }

        if (biggest_army_holder != -1){
            sums[biggest_army_holder] += 2;
            cout << "biggest_army_holder is: " << players[biggest_army_holder]->name << endl;
        }

        int roades[3] = {0,0,0};
        for (int i = 0; i < 144; i++){
            if(board.edges[i].players_road != 0){
               roades[board.edges[i].players_road-1] ++; 
            }
        }
        int longest_road = 0;
        int longest_road_holder = -1;        
        for (int i = 0; i < 3; i++){
            if(longest_road < roades[i]){
                longest_road_holder = i;
            }
        }
        if (longest_road_holder != -1){
            sums[longest_road_holder] += 2;
            cout << "longest_road_holder is: " << players[longest_road_holder]->name << endl;
        }
 
        for (int i = 0; i < 54; i++){
            int p = board.nodes[i].players_city;
            if(p!=0){
                if(p<4){
                    sums[p-1]++;
                }else{
                    p = p-3;
                    sums[p] += 2;
                }
            }
        }
        

        for (int i = 0; i < 3; i++){
            sums[i] += players[i]->development_cards[0];
        }
        
        int winner = -1;
        int sum=0;
        for (int i = 0; i < 3; i++){
            if(sum<sums[i]){
                sum = sums[i];
                winner = i;
            }
        }
        if (winner != -1){
            cout << "the winner is: " << players[winner]->name << endl;
        }
        }

    string landscapes[6] = {
        "forest",  "pasture land", "agricaltural land",
        "mountains","hills", "desert"
    };

    string resources[6] = {
        "wood", "wool", "wheat", "ore", "bricks", ""
    };

    //each node called by its hexagon letter. hexas with 2 single-lettered nodes hadded 0/1 clockwise
    string names[54] = {
        "a0","a1","ab","ad","ade","abe",
        "b","bc","bcf","bef",
        "c0","c1","cg","cfg",
        "d","dei","dhi","dh",
        "eij","efj",
        "fjk","fgk",
        "g","gl","glk",
        "h0","h1","him","hm",
        "ijn","imn",
        "jko","jno",
        "klp","kop",
        "l0","l1","lp",
        "m","mnq","mq",
        "nor","nqr",
        "ops","ors",
        "p","ps",
        "q0","q1","qr",
        "r","rs",
        "s0","s1"
    };

    string additional_edges[24] = {
        "a0","ad",
        "a1","ab",
        "c0","bc",
        "c1","cg",
        "h0","hm",
        "h1","dh",
        "l0","gl",
        "l1","lp",
        "q0","qr",
        "q1","mq",
        "s0","ps",
        "s1","rs"
    };


    Hexa new_hexa(Hexa hexa, int i, int type, int number){
        char letter = 'a' + i;
        hexa.letter = std::string(1, letter);
        hexa.landscape = landscapes[type];
        hexa.resource = resources[type];
        hexa.number = number;
        hexa.res_num = type;
        if (number == 0){
            hexa.has_knight = 1;
        }else{
            hexa.has_knight = 0;
        }
        return hexa;
    }

    Edge new_edge(Edge edge, string namei, string namej){
        edge.first = namei;
        edge.second = namej;
        edge.players_road = 0;
        return edge;
    }

    Node* createNodes() {
        Node* nodes = new Node[54];
        // Populate nodes as needed
        for (int i = 0; i < 54; ++i) {
            nodes[i].name = names[i];
            nodes[i].players_city = 0;    }
        return nodes;
    }

    Edge* createEdges(){
        Edge* edges = new Edge[144];
        int counter = 0;
        for (int i = 0; i < 54; i++) {
            for (int j = i + 1; j < 54; j++) {
                int matchingLetters = 0;
                if(names[i].length() == 3){
                    for (int k = 0; k < 3; k++) {
                        if (names[j].find(names[i][k]) != string::npos) {
                            matchingLetters++;
                        }
                    } 
                    if (matchingLetters == 2) {
                        edges[counter] = new_edge(edges[counter], names[i], names[j]);
                        counter++;                    
                        edges[counter] = new_edge(edges[counter], names[j], names[i]);
                        counter++;
                    }   
                }
                if(names[i].length() == 2){
                    for (int k = 0; k < 2; k++) {
                        if (names[j].find(names[i][k]) != string::npos) {
                            matchingLetters++;
                        }
                    } 
                    if (matchingLetters == 2) {
                        edges[counter] = new_edge(edges[counter], names[i], names[j]);
                        counter++;                    
                        edges[counter] = new_edge(edges[counter], names[j], names[i]);
                        counter++;                }                
                    if (matchingLetters == 1){
                        if(names[j].length() == 1){
                            edges[counter] = new_edge(edges[counter], names[i], names[j]);
                            counter++;                    
                            edges[counter] = new_edge(edges[counter], names[j], names[i]);
                            counter++;                    }
                        if(names[j].length() == 2 &&
                        names[i][0] == names[j][0]&&
                        names[j][1] == '1'){
                            edges[counter] = new_edge(edges[counter], names[i], names[j]);
                            counter++;                    
                            edges[counter] = new_edge(edges[counter], names[j], names[i]);
                            counter++;                    }
                    }
                }
                if(names[i].length() == 1){
                    if(names[j].length() == 2 &&
                    names[j].find(names[i][0]) != string::npos){
                        edges[counter] = new_edge(edges[counter], names[i], names[j]);
                        counter++;                    
                        edges[counter] = new_edge(edges[counter], names[j], names[i]);
                        counter++;
                    } 
                }
            }
        }
        for (int i = 0; i < 12; i++){
            edges[counter] = new_edge(edges[counter], additional_edges[i*2], additional_edges[i*2+1]);
            counter++;                    
            edges[counter] = new_edge(edges[counter], additional_edges[i*2+1], additional_edges[i*2]);
            counter++; 
        }
        return edges;
    }

Hexa* createHexas() {
    Hexa* hexas = new Hexa[19];
    int shuffle_landscape[19] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(shuffle_landscape, shuffle_landscape + 19, gen);
    
    int arr_shuffle_numbers[18] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    std::shuffle(arr_shuffle_numbers, arr_shuffle_numbers + 18, gen);
    
    int number_index = 0;
    
    for (int i = 0; i < 19; ++i) {
        int type = shuffle_landscape[i];
        int number = (type == 5) ? 0 : arr_shuffle_numbers[number_index++];
        hexas[i] = new_hexa(hexas[i], i, type, number);
    }
    
    return hexas;
}

    Board createBoard() {
        Board board; 
        board.nodes = createNodes();
        board.edges = createEdges(); 
        board.hexas = createHexas();
        return board;
    }
};



int main() {
    Player p1("itamar");
    Player p2("maayan");
    Player p3("coputador");
    Catan catan(p1, p2, p3);
    Board board = catan.getBoard();
    catan.ChooseStartingPlayer();
    
    // i want the normal order
    
    if(p2.my_turn == 1){
        p2.endTrurn();
        if(p3.my_turn == 1){
            p3.endTrurn();
        }  
    }
    if(p3.my_turn == 1){
        p3.endTrurn();
        if(p2.my_turn == 1){
            p2.endTrurn();
        }  
    }

// hexas[1] is b
// hexas[2] is c
    vector<string> places = {board.hexas[1].landscape, board.hexas[2].landscape};
    vector<int> placesNum = {board.hexas[1].number, board.hexas[2].number};
    p1.placeRoad(places, placesNum, board);
    p1.placeSettelemnt(places, placesNum, board);
    while(!board.nodes[7].players_city){sleep(1);}
    
    
    cout << catan.players[0]->name << "'s number is: " <<catan.players[0]->number << endl;
    
//nodes[7] is bc
    cout << "settelment named: " <<  board.nodes[7].name << " belongs to " 
    << catan.players[board.nodes[7].players_city-1]->name <<  endl;
    
//  edges[18] bc -> bcf
    cout << "road named: " << "  " <<  board.edges[18].first
    << "  " << board.edges[18].second << "  belongs to " 
    << catan.players[board.edges[18].players_road-1]->name << endl;

    for (int i = 0; i < 19; i++){
        cout << "hexagon named: " << board.hexas[i].letter << " has the number: " 
        << board.hexas[i].number << " and the resource: " 
        << board.hexas[i].resource << endl;
    }



    for (size_t i = 0; i < 40; i++){
        p1.rollDice(board);
        p1.endTrurn();
        if(p2.my_turn == 1){
            p2.rollDice(board);
            p2.endTrurn();
            p3.rollDice(board);        
            p3.endTrurn();
        }else{
            p3.rollDice(board);        
            p3.endTrurn();
            p2.rollDice(board);
            p2.endTrurn();
        } 
    }
    p1.buyDevelopmentCard();
    p1.buyDevelopmentCard();
    p1.buyDevelopmentCard();
    p1.useKnight(board,"maayan", board.hexas[0].landscape, board.hexas[0].number, 0);
    p1.printCards();
    p2.printCards();
    p3.printCards();
    catan.printWinner();
    return 0;
}
