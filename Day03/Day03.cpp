#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>

int toInt(std::string s);
bool checkForSymbols(std::vector<std::string> engine, int start, int end, int lineNum, int number);
bool isNotPointOrNumber(char c);
void addStar(int x, int y, int number);

struct STAR {
    int starX, starY;
    int amountOfNumbers;
    std::vector<int> numbers;
};

std::vector<STAR> stars;

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
    int sum = 0, it;
    std::string number = "";
    std::vector<std::string> engineLines;
    bool atDigit = false, nextToSymbol = false;
    int numberStartPos = 0, numberEndPos = 0, numberLine = 0;

    /* READ ENGINE INFO */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {
        if (nextLine == "") { continue; }
        engineLines.push_back(nextLine);        
    }

    /* LOOK FOR NUMBERS IN ENGINE */

    for (std::string line : engineLines) {

        for (it = 0; it < line.length(); it++) {

            if (isdigit(line[it])) {
                if (!atDigit) {
                    numberStartPos = it;
                }
                number += line[it];
                atDigit = true;
            }
            else {
                if (atDigit) {
                    numberEndPos = it - 1;
                    nextToSymbol = checkForSymbols(engineLines, numberStartPos, numberEndPos, numberLine, toInt(number));
                    if (nextToSymbol) {
                        //sum += toInt(number);
                    }
                    number = "";
                }
                atDigit = false;
            }

            if (it == line.length() - 1 && atDigit) {
                numberEndPos = it - 1;
                nextToSymbol = checkForSymbols(engineLines, numberStartPos, numberEndPos, numberLine, toInt(number));
                if (nextToSymbol) {
                    //sum += toInt(number);
                }
                number = "";
                atDigit = false;
            }
        }
        numberLine++;
    }

    int counter = 1;
    bool foundDouble = false;
    std::vector<STAR> doubleStars;
    for (STAR star : stars) {

        if (star.amountOfNumbers == 2) {
            doubleStars.push_back(star);
        }
    }

    for (STAR star : doubleStars) {
        
        /*
        std::cout << "Star Nr." << counter << " - x: " << star.starX << " - y: " << star.starY << " #numbers: " << star.amountOfNumbers << " - numbers-list: ";
        for (int i : star.numbers) {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
        */

        sum += star.numbers.at(0) * star.numbers.at(1);

        counter++;
    }

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 03" << std::endl;
    std::cout << "Sum of the power of all sets: " << sum << std::endl;

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
            std::cout << "Bad Input!" << std::endl;
            exit(1);
        }
    }
    return i;
}

bool checkForSymbols(std::vector<std::string> engine, int start, int end, int lineNum, int number) {

    // std::cout << "number: " << number << " - stars: " << stars.size() << std::endl;

    /* DEFINE RELEVANT LINES */
    std::string lineAbove, lineBelow, line;
    if (lineNum != 0) {
        lineAbove = engine.at(lineNum - 1);
    }
    line = engine.at(lineNum);
    if (lineNum != engine.size() - 1) {
        lineBelow = engine.at(lineNum + 1);
    }

    /* CHECKING FOR SYMBOLS != '.' AROUND THE NUMBER */
    if (start != 0 && isNotPointOrNumber(line[start - 1])) {
     
        if (line[start - 1] == '*') {
            addStar(start - 1, lineNum, number);
        }
        
        return true;
    }
    if (end != line.length() && isNotPointOrNumber(line[end + 1])) {

        if (line[end + 1] == '*') {
            addStar(end + 1, lineNum, number);
        }

        return true;
    }
    if (start != 0) {
        start--;
    }
    if (end != line.length()) {
        end++;
    }
    for (int it = start; it <= end; it++) {
        if (lineNum != 0 && isNotPointOrNumber(lineAbove[it])) {

            if (lineAbove[it] == '*') {
                addStar(it, lineNum-1, number);
            }

            return true;
        }
        if (lineNum != engine.size() - 1 && isNotPointOrNumber(lineBelow[it])) {

            if (lineBelow[it] == '*') {
                addStar(it, lineNum+1, number);
            }

            return true;
        }
    }

    return false;
}

bool isNotPointOrNumber(char c) {

    if (c != '.' && !isdigit(c)) {
        return true;
    }

    return false;
}

void addStar(int x, int y, int number) {

    for (STAR star : stars) {

        if (star.starX == x && star.starY == y) {
            star.amountOfNumbers++;
            star.numbers.push_back(number);
            stars.push_back(star);
            return;
        }
    }

    STAR star;
    star.amountOfNumbers = 1;
    star.numbers.push_back(number);
    star.starX = x;
    star.starY = y;
    stars.push_back(star);
}