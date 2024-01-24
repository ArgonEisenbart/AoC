#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

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
    int const MAX_RED = 12, MAX_GREEN = 13, MAX_BLUE = 14;
    int red = 0, green = 0, blue = 0, nextBallAmount = 0, color = 0, sum = 0, it, pos;
    std::string gameNr, balls, nextBallString;

    /* LOOPING */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {
        if (nextLine == "") { continue; }

        std::string gameNr = "";
        it = 5;
        while (isdigit(nextLine[it])) {
            gameNr += nextLine[it];
            it++;
        }

        balls = nextLine.substr(6 + gameNr.length(), nextLine.length());
        pos = 0;
        while (pos < balls.length()) {

            std::string numberString = "";
            it = pos + 1;
            while (isdigit(balls[it])) {
                numberString += balls[it];
                it++;
            }
            nextBallAmount = toInt(numberString);
            
            switch (balls[pos + 2 + numberString.length()])
            {
            case 'r':
                color = 3;
                if (red == 0) {
                    red = nextBallAmount;
                }
                else if (nextBallAmount > red) {
                    red = nextBallAmount;
                }
                break;
            case 'g':
                color = 5;
                if (green == 0) {
                    green = nextBallAmount;
                }
                else if (nextBallAmount > green) {
                    green = nextBallAmount;
                }
                break;
            case 'b':
                color = 4;
                if (blue == 0) {
                    blue = nextBallAmount;
                }
                else if (nextBallAmount > blue) {
                    blue = nextBallAmount;
                }
                break;
            }
            pos += color + 3 + numberString.length();
        }

        sum += red * green * blue;

        /*if (red <= MAX_RED && green <= MAX_GREEN && blue <= MAX_BLUE) {
            sum += toInt(gameNr);
        }*/

        // std::cout << "Game Nr. " << gameNr << " : red: " << red << " - blue: " << blue << " - green: " << green << std::endl;

        red = 0;
        green = 0;
        blue = 0;
        nextBallAmount = 0;
    }

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 02" << std::endl;
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