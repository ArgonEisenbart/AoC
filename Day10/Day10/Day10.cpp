#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

int toInt(std::string s);
int followPipe(int direction, char symbol);
bool isVertical(char symbol);
bool isHorizontal(char symbol);
bool isCorner(char symbol);
bool isLeft(char symbol);
bool isRight(char symbol);
bool isUp(char symbol);
bool isDown(char symbol);

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
    int sum = 0, startX = 0, startY = 0;
    std::vector<std::vector<char>> pipeMap;

    /* READ MAP */
    std::string nextLine;
    int row = 0;
    while (std::getline(myfile, nextLine)) {
        std::vector<char> nextChars;
        for (int i = 0; i < nextLine.length(); i++) {
            nextChars.push_back(nextLine[i]);
            if (nextLine[i] == 'S') {
                startX = i;
                startY = row;
            }
        }
        pipeMap.push_back(nextChars);
        row++;
    }

    std::vector<std::vector<int>> markMap;  // 0 = initialised , 1 = pipe loop , 3 = inside, 4 = outside
    for (int i = 0; i < pipeMap.size(); i++) {
        std::vector<int> newRow;
        for (int j = 0; j < pipeMap.at(i).size(); j++) {
            newRow.push_back(0);
        }
        markMap.push_back(newRow);
    }

    int currentX = startX + 1;
    int currentY = startY;
    int direction = 1;  // 0 = up, 1 = right, 2 = down, 3 = left
    char symbol = pipeMap.at(currentY).at(currentX);
    markMap.at(currentY).at(currentX) = 1;
    int pipesCounted = 1;

    while (symbol != 'S') {
        direction = followPipe(direction, symbol);
        switch (direction)
        {
        case 0:
            currentX = currentX;
            currentY = currentY - 1;
            break;
        case 1:
            currentX = currentX + 1;
            currentY = currentY;
            break;
        case 2:
            currentX = currentX;
            currentY = currentY + 1;
            break;
        case 3:
            currentX = currentX - 1;
            currentY = currentY;
            break;
        }

        if (direction == -1) {
            std::cout << "invalid follow!" << std::endl;
            exit(1);
        }

        if (direction == 4) {
            std::cout << "back at S! steps:" << pipesCounted << std::endl;
        }

        symbol = pipeMap.at(currentY).at(currentX);
        markMap.at(currentY).at(currentX) = 1;
        pipesCounted++;
    }

    pipesCounted = pipesCounted / 2;

    // --------- 2 --------------------

    // change S to a symbol
    pipeMap.at(startY).at(startX) = 'F';

    int countedInsides = 0;
    int wallsN = 0, wallsE = 0, wallsS = 0, wallsW = 0;
    bool prolongingWallN = false, prolongingWallE = false, prolongingWallS = false, prolongingWallW = false;
    std::string comingFromHorizontally = "", comingFromVertically = "";
    for (int i = 0; i < pipeMap.size(); i++) {
        for (int j = 0; j < pipeMap.at(i).size(); j++) {
            if (markMap.at(i).at(j) == 1) {
                continue;
            }

            // links
            for (int k = j - 1; k >= 0; k--) {
                char currentSymbol = pipeMap.at(i).at(k);
                if (markMap.at(i).at(k) == 1) {
                    if (isVertical(currentSymbol)) {
                        wallsW++;
                    }
                    else if (isCorner(currentSymbol)) {
                        if (prolongingWallW) {
                            prolongingWallW = false;
                            if (comingFromVertically == "up" && isUp(currentSymbol)) {
                                wallsW = wallsW + 2;
                            }
                            else if (comingFromVertically == "up" && isDown(currentSymbol)) {
                                wallsW = wallsW + 1;
                            }
                            else if (comingFromVertically == "down" && isDown(currentSymbol)) {
                                wallsW = wallsW + 2;
                            }
                            else if (comingFromVertically == "down" && isUp(currentSymbol)) {
                                wallsW = wallsW + 1;
                            }
                            comingFromVertically = "";
                        }
                        else {
                            prolongingWallW = true;
                            if (isUp(currentSymbol)) {
                                comingFromVertically = "up";
                            }
                            else {
                                comingFromVertically = "down";
                            }
                        }
                    }
                }
            }

            // rechts
            for (int k = j + 1; k < markMap.at(i).size(); k++) {
                int currentSymbol = pipeMap.at(i).at(k);
                if (markMap.at(i).at(k) == 1) {
                    if (isVertical(currentSymbol)) {
                        wallsE++;
                    }
                    else if (isCorner(currentSymbol)) {
                        if (prolongingWallE) {
                            prolongingWallE = false;
                            if (comingFromVertically == "up" && isUp(currentSymbol)) {
                                wallsE = wallsE + 2;
                            }
                            else if (comingFromVertically == "up" && isDown(currentSymbol)) {
                                wallsE = wallsE + 1;
                            }
                            else if (comingFromVertically == "down" && isDown(currentSymbol)) {
                                wallsE = wallsE + 2;
                            }
                            else if (comingFromVertically == "down" && isUp(currentSymbol)) {
                                wallsE = wallsE + 1;
                            }
                            comingFromVertically = "";
                        }
                        else {
                            prolongingWallE = true;
                            if (isUp(currentSymbol)) {
                                comingFromVertically = "up";
                            }
                            else {
                                comingFromVertically = "down";
                            }
                        }
                    }
                }
            }
            
            // oben
            for (int k = i - 1; k >= 0; k--) {
                int currentSymbol = pipeMap.at(k).at(j);
                if (markMap.at(k).at(j) == 1) {
                    if (isHorizontal(currentSymbol)) {
                        wallsN++;
                    }
                    else if (isCorner(currentSymbol)) {
                        if (prolongingWallN) {
                            prolongingWallN = false;
                            if (comingFromHorizontally == "left" && isLeft(currentSymbol)) {
                                wallsN = wallsN + 2;
                            }
                            else if (comingFromHorizontally == "left" && isRight(currentSymbol)) {
                                wallsN = wallsN + 1;
                            }
                            else if (comingFromHorizontally == "right" && isRight(currentSymbol)) {
                                wallsN = wallsN + 2;
                            }
                            else if (comingFromHorizontally == "right" && isLeft(currentSymbol)) {
                                wallsN = wallsN + 1;
                            }
                            comingFromHorizontally = "";
                        }
                        else {
                            prolongingWallN = true;
                            if (isLeft(currentSymbol)) {
                                comingFromHorizontally = "left";
                            }
                            else {
                                comingFromHorizontally = "right";
                            }
                        }
                    }
                }
            }
            
            // unten
            for (int k = i + 1; k < markMap.size(); k++) {
                int currentSymbol = pipeMap.at(k).at(j);
                if (markMap.at(k).at(j) == 1) {
                    if (isHorizontal(currentSymbol)) {
                        wallsS++;
                    }
                    else if (isCorner(currentSymbol)) {
                        if (prolongingWallS) {
                            prolongingWallS = false;
                            if (comingFromHorizontally == "left" && isLeft(currentSymbol)) {
                                wallsS = wallsS + 2;
                            }
                            else if (comingFromHorizontally == "left" && isRight(currentSymbol)) {
                                wallsS = wallsS + 1;
                            }
                            else if (comingFromHorizontally == "right" && isRight(currentSymbol)) {
                                wallsS = wallsS + 2;
                            }
                            else if (comingFromHorizontally == "right" && isLeft(currentSymbol)) {
                                wallsS = wallsS + 1;
                            }
                            comingFromHorizontally = "";
                        }
                        else {
                            prolongingWallS = true;
                            if (isLeft(currentSymbol)) {
                                comingFromHorizontally = "left";
                            }
                            else {
                                comingFromHorizontally = "right";
                            }
                        }
                    }
                }
            }

            if ((wallsN > 0 && wallsE > 0 && wallsS > 0 && wallsW > 0) && (wallsN % 2 == 1 || wallsE % 2 == 1 || wallsS % 2 == 1 || wallsW % 2 == 1)) {
                countedInsides++;
            }

            std::cout << "(symbol: " << pipeMap.at(i).at(j) << " y: " << i << " x: " << j << ") - " << "N: " << wallsN << " E: " << wallsE << " S: " << wallsS << " W: " << wallsW << " count: " << countedInsides << std::endl;

            wallsN = 0;
            wallsE = 0;
            wallsS = 0;
            wallsW = 0;
            prolongingWallN = false;
            prolongingWallE = false;
            prolongingWallS = false;
            prolongingWallW = false;
            comingFromHorizontally = "";
            comingFromVertically = "";
        }
    }

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 10" << std::endl;
    std::cout << "Total inside: " << countedInsides << std::endl;

    /* TIMER */
    auto time_end = std::chrono::system_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Total time needed: " << totalTime.count() << " ms\n";
}

int toInt(std::string s) {

    bool minusNumber = false;
    if (s[0] == '-') {
        minusNumber = true;
        s = s.substr(1, s.length());
    }

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
    if (!minusNumber) {
        return i;
    }
    else {
        return (-1) * i;
    }
}

int followPipe(int direction, char symbol) {    //returns nextDirection
    switch (direction)
    {
    case 0:
        switch (symbol)
        {
        case '|':
            return 0;
        case '-':
            return -1;
        case 'L':
            return -1;
        case 'J':
            return -1;
        case '7':
            return 3;
        case 'F':
            return 1;
        case '.':
            return -1;
        case 'S':
            return 4;
        }
        break;
    case 1:
        switch (symbol)
        {
        case '|':
            return -1;
        case '-':
            return 1;
        case 'L':
            return -1;
        case 'J':
            return 0;
        case '7':
            return 2;
        case 'F':
            return -1;
        case '.':
            return -1;
        case 'S':
            return 4;
        }
        break;
    case 2:
        switch (symbol)
        {
        case '|':
            return 2;
        case '-':
            return -1;
        case 'L':
            return 1;
        case 'J':
            return 3;
        case '7':
            return -1;
        case 'F':
            return -1;
        case '.':
            return -1;
        case 'S':
            return 4;
        }
        break;
    case 3:
        switch (symbol)
        {
        case '|':
            return -1;
        case '-':
            return 3;
        case 'L':
            return 0;
        case 'J':
            return -1;
        case '7':
            return -1;
        case 'F':
            return 2;
        case '.':
            return -1;
        case 'S':
            return 4;
        }
        break;
    }
}

bool isVertical(char symbol) {
    switch (symbol)
        {
        case '|':
            return true;
        case '-':
            return false;
        case 'L':
            return false;
        case 'J':
            return false;
        case '7':
            return false;
        case 'F':
            return false;
        case 'S':
            return false;
        }
}

bool isHorizontal(char symbol) {
    switch (symbol)
    {
    case '|':
        return false;
    case '-':
        return true;
    case 'L':
        return false;
    case 'J':
        return false;
    case '7':
        return false;
    case 'F':
        return false;
    case 'S':
        return false;
    }
}

bool isCorner(char symbol) {
    switch (symbol)
    {
    case '|':
        return false;
    case '-':
        return false;
    case 'L':
        return true;
    case 'J':
        return true;
    case '7':
        return true;
    case 'F':
        return true;
    case 'S':
        return true;
    }
}

bool isLeft(char symbol) {
    switch (symbol)
    {
    case '|':
        return false;
    case '-':
        return false;
    case 'L':
        return false;
    case 'J':
        return true;
    case '7':
        return true;
    case 'F':
        return false;
    }
}

bool isRight(char symbol) {
    switch (symbol)
    {
    case '|':
        return false;
    case '-':
        return false;
    case 'L':
        return true;
    case 'J':
        return false;
    case '7':
        return false;
    case 'F':
        return true;
    }
}

bool isUp(char symbol) {
    switch (symbol)
    {
    case '|':
        return false;
    case '-':
        return false;
    case 'L':
        return true;
    case 'J':
        return true;
    case '7':
        return false;
    case 'F':
        return false;
    }
}

bool isDown(char symbol) {
    switch (symbol)
    {
    case '|':
        return false;
    case '-':
        return false;
    case 'L':
        return false;
    case 'J':
        return false;
    case '7':
        return true;
    case 'F':
        return true;
    }
}