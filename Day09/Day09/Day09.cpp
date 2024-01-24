#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

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
    std::vector<int> topLineNumbers;

    /* LOOPING */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {

        while (nextLine != "") {
            std::string nextNumberString = "";
            int pos = 0;
            while (pos < nextLine.length() && nextLine[pos] != ' ') {
                nextNumberString += nextLine[pos];
                pos++;
            }
            int nextNumber = toInt(nextNumberString);
            topLineNumbers.push_back(nextNumber);
            if (pos == nextLine.length()) {
                nextLine = "";
            }
            else {
                nextLine = nextLine.substr(pos + 1, nextLine.length());
            }
        }

        bool finished;
        int startToAdd = 0;
        for (int scope = topLineNumbers.size(); scope > 0; scope--) {

            finished = true;
            for (int i = topLineNumbers.size() - scope; i < topLineNumbers.size(); i++) {
                if (topLineNumbers[i] != 0) {
                    finished = false;
                    break;
                }
            }

            if (!finished) {
                for (int i = topLineNumbers.size() - 1; i > topLineNumbers.size() - scope; i--) {
                    topLineNumbers.at(i) = topLineNumbers[i] - topLineNumbers[i - 1];
                }
            }
            else {
                startToAdd = topLineNumbers.size() - scope;
                break;
            }
        }

        int lineSum = 0;
        for (int i = startToAdd; i >= 0; i--) {
            lineSum = topLineNumbers[i] - lineSum;
        }
        sum += lineSum;
        
        std::vector<int> newTopLineNumbers;
        topLineNumbers = newTopLineNumbers;
    }

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 09" << std::endl;
    std::cout << "Total steps: " << sum << std::endl;

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