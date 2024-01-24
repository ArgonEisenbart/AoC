#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>

int toInt(std::string s);

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
    int sum = 0, cardPoints, cardNr, amountOfWinningNumbers = 0, countLines = 0;
    std::string cardString, winningNumbersString, elfNumbersString;
    std::vector<int> winningNumbers, elfNumbers, winNumbersCount;

    /* LOOPING */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {

        cardString = nextLine.substr(0, nextLine.find(":"));
        winningNumbersString = nextLine.substr(cardString.length() + 2, nextLine.find(" | ") - (cardString.length() + 2));
        elfNumbersString = nextLine.substr(cardString.length() + 2 + winningNumbersString.length() + 3, nextLine.length());
        int pos = 4;
        while (cardString[pos] == ' ') {
            pos++;
        }
        cardString = cardString.substr(pos, cardString.length());
        cardNr = toInt(cardString);

        while (winningNumbersString != "") {
            while (winningNumbersString.substr(0,1) == " ") {
                winningNumbersString = winningNumbersString.substr(1, winningNumbersString.length());
            }
            std::string nextNumber = winningNumbersString.substr(0, winningNumbersString.find(" "));
            winningNumbersString = winningNumbersString.substr(nextNumber.length(), winningNumbersString.length());
            winningNumbers.push_back(toInt(nextNumber));
        }

        while (elfNumbersString != "") {
            while (elfNumbersString.substr(0, 1) == " ") {
                elfNumbersString = elfNumbersString.substr(1, elfNumbersString.length());
            }
            std::string nextNumber = elfNumbersString.substr(0, elfNumbersString.find(" "));
            elfNumbersString = elfNumbersString.substr(nextNumber.length(), elfNumbersString.length());
            elfNumbers.push_back(toInt(nextNumber));
        }

        for (int elfNumber : elfNumbers) {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), elfNumber) != winningNumbers.end()) {
                amountOfWinningNumbers++;
            }
        }

        winNumbersCount.push_back(amountOfWinningNumbers);

        /*
        if (amountOfWinningNumbers == 0) {
            cardPoints = 0;
        }
        else {
            cardPoints = 1;
            amountOfWinningNumbers--;
            while (amountOfWinningNumbers > 0) {
                cardPoints *= 2;
                amountOfWinningNumbers--;
            }
        }
        sum += cardPoints;
        */

        cardPoints = 0;
        amountOfWinningNumbers = 0;
        winningNumbers.clear();
        elfNumbers.clear();
        countLines++;
    }

    int card = 0, cardsCounter = 0;
    int* amountOfCards = new int[countLines];
    for (int i = 0; i < countLines; i++) {
        amountOfCards[i] = 1;
    }

    for (int winNumber : winNumbersCount) {

        cardsCounter = amountOfCards[card];
        while (cardsCounter > 0) {
            for (int i = card + 1; i <= (card + winNumber); i++) {
                if (i < countLines) {
                    amountOfCards[i]++;
                }
            }
            cardsCounter--;
        }

        sum += amountOfCards[card];

        card++;
    }
    delete[] amountOfCards;

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