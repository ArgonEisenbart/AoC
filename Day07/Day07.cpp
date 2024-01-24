#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>

int toInt(std::string s);

struct Hand {
    Hand(char card1, char card2, char card3, char card4, char card5) {
        card[0] = card1;
        card[1] = card2;
        card[2] = card3;
        card[3] = card4;
        card[4] = card5;
    }
    char card[5] = { 0, 0, 0, 0, 0 };
    int value;
};

int main()
{
    auto time_start = std::chrono::system_clock::now();

    /* OPEN STREAM */
    std::ifstream myfile;
    myfile.open("ressources/input.txt");
    if (!myfile) {
        std::cerr << "Error: file count not be opened" << std::endl;
        exit(1);
    }

    /* DEFINE VARIBLES*/
    int sum = 0, totalHands = 0;
    std::vector<Hand> hands;
    std::vector<int> bids;

    /* LOOPING */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {

        Hand nextHand = Hand(nextLine[0], nextLine[1], nextLine[2], nextLine[3], nextLine[4]);
        nextHand.value = calculateHandValue(nextHand);
        totalHands++;
        hands.push_back(nextHand);
        bids.push_back(toInt(nextLine.substr(6, nextLine.length())));
    }

    /**/
    for (int i = 0; i < totalHands; i++) {


    }


    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 04" << std::endl;
    std::cout << "Sum of all the cards: " << sum << std::endl;

    /* TIMER */
    auto time_end = std::chrono::system_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Total time needed: " << totalTime.count() << " ms\n";
}

int toInt(std::string s) {

    int i = 0;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            i = i * 10 + (c - '0');
        }
        else {
            std::cout << "Bad Input! Char: '" << c << "'" << std::endl;
            exit(1);
        }
    }
    return i;
}

/*
*   7 = five of a kind
*   6 = four of a kind
*   5 = full house
*   4 = three of a kind
*   3 = two pair
*   2 = one pair
*   1 = high card
*/
int calculateHandValue(Hand nextHand) {

    char card1 = nextHand.card[0];
    char card2 = nextHand.card[1];
    char card3 = nextHand.card[2];
    char card4 = nextHand.card[3];
    char card5 = nextHand.card[4];

    bool containsPair = false;

    if (card1 == card2 || card1 == card3 || card1 == card4 || card1 == card5 ||
                          card2 == card3 || card2 == card4 || card2 == card5 ||
                                            card3 == card4 || card3 == card5 ||
                                                              card4 == card5) {

        containsPair = true;
    }


    if (card1 == card2 && card2 == card3 && card3 == card4 && card4 == card5) {
        return 7;
    }





    return -1;
}