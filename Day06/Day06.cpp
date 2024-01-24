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
    int sum = 0;
    long long time[1]     { 41777096 };
    long long distance[1] { 249136211271011 };
    long long raceWins[1] { 0 };
    long long t1, t2, chargeTime, moveTime, chargeSpeed, ownRaceDistance;

    for (int race = 0; race < 1; race++) {
        for (int i = 0; i < time[race]; i++) {
            chargeSpeed = i;
            moveTime = time[race] - i;
            ownRaceDistance = chargeSpeed * moveTime;

            //std::cout << "raceDistance: " << ownRaceDistance << std::endl;

            if (ownRaceDistance > distance[race]) {
                raceWins[race]++;
            }
        }
    }
    std::cout << raceWins[0] << std::endl;

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 06" << std::endl;
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