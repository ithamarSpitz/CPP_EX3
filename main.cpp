#include "Catan.hpp"
#include "Player.hpp"
#include <unistd.h>
//#include <future>         // std::async, std::future

using namespace ariel;

void p1Start(Player p1, Player p2, Player p3)
{
    // rearrange the loop to start with p1

    if (p2.my_turn == 1)
    {
        p2.endTrurn();
        if (p3.my_turn == 1)
        {
            p3.endTrurn();
        }
    }
    if (p3.my_turn == 1)
    {
        p3.endTrurn();
        if (p2.my_turn == 1)
        {
            p2.endTrurn();
        }
    }
}

void printBoard(Board board)
{
    for (int i = 0; i < 19; i++)
    {
        cout << "hexagon named: " << board.hexas[i].letter << " has the number: "
             << board.hexas[i].number << " and the resource: "
             << board.hexas[i].resource << endl;
    }
}

void rollNskip(Board board, Player p1, Player p2, Player p3, int times)
{
    for (int i = 0; i < times; i++)
    {
        p1.rollDice(board);
        p1.endTrurn();
        if (p1.my_turn == 1)
        {
            sleep(1);
        }
        if (p2.my_turn == 1)
        {
            p2.rollDice(board);
            p2.endTrurn();
            if(p3.my_turn != 1)
            {
                sleep(1);
            }
            p3.rollDice(board);
            p3.endTrurn();
        }
        else
        {
            p3.rollDice(board);
            p3.endTrurn();
            if(p2.my_turn != 1)
            {
                sleep(1);
            }
            p2.rollDice(board);
            p2.endTrurn();
        }
    }
}

void updating(Board board, bool is_edge, int number){
    if(is_edge)
    {
        if (!board.edges[number].players_road)
        {
            sleep(1);
        }
    }else
    {
        if (!board.nodes[number].players_city)
        {
            sleep(1);
        }
    }
}

void start(Catan& catan, Board& board, Player& p1, Player& p2, Player& p3){    for (int i = 0; i < 100; i++)
    {
        Player pl1("itamar");
        Player pl2("maayan");
        Player pl3("coputador");
            cout <<"is it"<< endl;

        Catan catanl(pl1, pl2, pl3);
            cout <<"is it"<< endl;

        Board boardl = catanl.getBoard();
    cout <<"is it"<< endl;

        catanl.ChooseStartingPlayer();
        sleep(1);
        cout << "1";
        if (catanl.players[0]->name == pl1.name &&
        catanl.players[1]->name == pl2.name &&
        catanl.players[2]->name == pl3.name){
            catan = catanl;
            board = boardl;
            p1 = pl1;
            p2 = pl2;
            p3 = pl3;
            break;
               
        }
    }
    

}


int main()
{

    Player p1 = Player();
    Player p2 = Player();
    Player p3 = Player();
    Catan catan = Catan();
    Board board = Board();
    //future<void> fut = async(start, std::ref(catan), std::ref(board), std::ref(p1), std::ref(p2), std::ref(p3));
    //fut.get();
    //catan.ChooseStartingPlayer();
    //p1Start(p1, p2, p3);
    // sleep(1);

    // hexas[1] is b
    // hexas[2] is c
    //vector<string> places = {board.hexas[1].landscape, board.hexas[2].landscape};
    //vector<int> placesNum = {board.hexas[1].number, board.hexas[2].number};
    // p1.placeRoad(places, placesNum, board);
    // updating(board, true, 18);

    // p1.placeSettelemnt(places, placesNum, board);
    // updating(board, false, 7);

    // cout << catan.players[0]->name << "'s number is: " << catan.players[0]->number << endl;

    // // nodes[7] is bc
    // cout << "settelment named: " << board.nodes[7].name << " belongs to "
    //      << catan.players[board.nodes[7].players_city - 1]->name << endl;

    // //  edges[18] bc -> bcf
    // cout << "road named: " << "  " << board.edges[18].first
    //      << "  " << board.edges[18].second << "  belongs to "
    //      << catan.players[board.edges[18].players_road - 1]->name << endl;

    // printBoard(board);

    // rollNskip(board, p1, p2, p3, 40);

    // p1.buyDevelopmentCard();
    // p1.buyDevelopmentCard();
    // p1.buyDevelopmentCard();
    // p1.useKnight(board, "maayan", board.hexas[0].landscape, board.hexas[0].number, 0);
    // p1.printCards();
    // p2.printCards();
    // p3.printCards();
    // catan.printWinner();
    return 0;

}
