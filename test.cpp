#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Catan.hpp"
#include "Player.hpp"

using namespace ariel;

int countCards(Player player){
        int count =0;
        for (int i = 0; i < 5; i++)
        {
            count += player.resources_cards[i];
        }
        return count;
}


TEST_CASE("Testing Player class") {
    Player player1("Alice");
    Player player2("Bob");
    Player player3("Charlie");

    CHECK(player1.name == "Alice");
    CHECK(player2.name == "Bob");
    CHECK(player3.name == "Charlie");
    Catan game(player1, player2, player3);
    game.ChooseStartingPlayer();
    Board board = game.getBoard();
    CHECK(board.nodes != nullptr);
    CHECK(board.edges != nullptr);
    CHECK(board.hexas != nullptr);
    SUBCASE("Test resource card removal") {
        int count =countCards(player1);
        int removed_card = player1.removeRandomNonZeroResourceCard();
        int count0 = countCards(player1);
        CHECK(removed_card != -1);
        CHECK(count == 1+count0);
    }

    SUBCASE("Test buying development card") {
        player1.resources_cards[0] = 1; // wood
        player1.resources_cards[1] = 1; // wool
        player1.resources_cards[2] = 1; // wheat
        player1.resources_cards[3] = 1; // wheat
        player1.resources_cards[4] = 1; // wheat

        player1.buyDevelopmentCard();
        CHECK((player1.development_cards[0] == 1 || player1.development_cards[1] == 1));
    }
    SUBCASE("Test rolling dice and getting resources") {
        // Ensure board is created correctly
        player1.my_turn = 1;
        player1.rollDice(board);
        CHECK(player1.dice_rolled == 1);
    }

    SUBCASE("Test end turn") {
        player1.my_turn = 1;
        player1.endTrurn();
        CHECK(player1.my_turn == 0);
    }

    SUBCASE("Test placing road") {
        player1.resources_cards[0] = 1; // wood
        player1.resources_cards[4] = 1; // brick
        player1.my_turn = 1;
        vector<string> places = {board.hexas[1].landscape, board.hexas[2].landscape};
        vector<int> placesNum = {board.hexas[1].number, board.hexas[2].number};
        player1.placeRoad(places, placesNum, board);
        CHECK(board.edges[18].players_road == player1.number);
    }

    SUBCASE("Test placing settlement") {
        player1.resources_cards[0] = 1; // wood
        player1.resources_cards[1] = 1; // wool
        player1.resources_cards[2] = 1; // wheat
        player1.resources_cards[4] = 1; // brick
        player1.my_turn = 1;
        board.edges[18].players_road = player1.number;
        vector<string> places = {board.hexas[1].landscape, board.hexas[2].landscape};
        vector<int> placesNum = {board.hexas[1].number, board.hexas[2].number};
        player1.placeSettelemnt(places, placesNum, board);
        CHECK(board.nodes[7].players_city == player1.number);
    }

    SUBCASE("Test placing city") {
        board.nodes[7].players_city = player1.number;
        player1.resources_cards[2] = 2; // wheat
        player1.resources_cards[3] = 3; // ore
        player1.my_turn = 1;
        vector<string> places = {board.hexas[1].landscape, board.hexas[2].landscape};
        vector<int> placesNum = {board.hexas[1].number, board.hexas[2].number};
        player1.placeCity(places, placesNum, board);
        CHECK(board.nodes[7].players_city == player1.number + 3);
    }



    SUBCASE("Test using knight card") {
        player1.development_cards[1] = 1; // knight
        player1.my_turn = 1;
        string robbed = "Bob";
        int boBfore = countCards(player2);
        string hexa_landscape = board.hexas[0].landscape;
        int hexa_num = board.hexas[0].number;
        int seven = 0;
        player1.useKnight(board, robbed, hexa_landscape, hexa_num, seven);
        CHECK(player1.development_cards[1] == 0);
        player2.removeRandomNonZeroResourceCard();
        int bAfter = countCards(player2);
        CHECK(boBfore != bAfter);
        CHECK(player1.army == 1);
        CHECK(board.hexas[0].has_knight == 1);
    }

    SUBCASE("Test exceptional behavior for settlement placement") {
        player1.resources_cards[0] = 0; // no wood
        player1.resources_cards[1] = 0; // no wool
        player1.resources_cards[2] = 0; // no wheat
        player1.resources_cards[4] = 0; // no brick
        player1.my_turn = 1;
        vector<string> places = {"forest", "hill"};
        vector<int> placesNum = {8, 4};
        player1.placeSettelemnt(places, placesNum, board);
        CHECK(board.nodes[0].players_city == 0); // No settlement should be placed
    }
}

TEST_CASE("Testing Catan class") {
    Player player1("Alice");
    Player player2("Bob");
    Player player3("Charlie");

    Catan game(player1, player2, player3);
    
    SUBCASE("Test ChooseStartingPlayer") {
        game.ChooseStartingPlayer();
        CHECK(player1.number != 0);
        CHECK(player2.number != 0);
        CHECK(player3.number != 0);
    }

    SUBCASE("Test board creation") {
        Board board = game.getBoard();
        CHECK(board.nodes != nullptr);
        CHECK(board.edges != nullptr);
        CHECK(board.hexas != nullptr);
    }

    SUBCASE("Test printWinner") {
        player1.army = 5;
        player2.army = 3;
        player3.army = 4;
        game.printWinner();
    }
}
