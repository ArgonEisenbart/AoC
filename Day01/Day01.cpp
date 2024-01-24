#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

int stringToDigit(std::string string);

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
    int firstDigit = 0, secondDigit = 0, sum = 0;
    bool firstDigitFound = false;
    std::string allowedDigits[18] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    /* LOOPING */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {
        if (nextLine == "") { continue; }
        for (int i = 0; i < nextLine.length(); i++) {
            std::string subLine = nextLine.substr(i, nextLine.length());
            for (std::string digit : allowedDigits) {
                if (subLine.substr(0, digit.length()) == digit) {
                    if (!firstDigitFound) {
                        firstDigit = stringToDigit(digit);
                        secondDigit = stringToDigit(digit);
                        firstDigitFound = true;
                    }
                    else {
                        secondDigit = stringToDigit(digit);
                    }
                }
            }
        }
        sum += 10 * firstDigit + secondDigit;

        // Reset für nächste Zeile
        int firstDigit = 0;
        int secondDigit = 0;
        firstDigitFound = false;
    }
    myfile.close();

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 01" << std::endl;
    std::cout << "Summe aller Zahlen: " << sum << std::endl;

    /* TIMER */
    auto time_end = std::chrono::system_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Total time needed: " << totalTime.count() << " ms\n";
}


int stringToDigit(std::string string) {

    if (string == "1" || string == "one")   { return 1; }
    if (string == "2" || string == "two")   { return 2; }
    if (string == "3" || string == "three") { return 3; }
    if (string == "4" || string == "four")  { return 4; }
    if (string == "5" || string == "five")  { return 5; }
    if (string == "6" || string == "six")   { return 6; }
    if (string == "7" || string == "seven") { return 7; }
    if (string == "8" || string == "eight") { return 8; }
    if (string == "9" || string == "nine")  { return 9; }

    return -1;
}