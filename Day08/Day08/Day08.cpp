#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

struct InstructionLetters {
    InstructionLetters(std::string from, std::string toLeft, std::string toRight) : from(from), toLeft(toLeft), toRight(toRight) {};
    std::string from;
    std::string toLeft;
    std::string toRight;
};

struct Instruction {
    Instruction(int from, int toLeft, int toRight) : from(from), toLeft(toLeft), toRight(toRight) {};
    int from;
    int toLeft;
    int toRight;
};

struct Step {
    Step(int at, int nextLeft, int nextRight, int stepsToGetHere) : at(at), nextLeft(nextLeft), nextRight(nextRight), stepsToGetHere(stepsToGetHere) {};
    int at;
    int nextLeft;
    int nextRight;
    int stepsToGetHere;
};

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
    std::vector<bool> directions;   // 0 = left, 1 = right

    /* READ SEQUENCE INPUT */
    std::string nextLine;
    std::getline(myfile, nextLine);

    bool nextDirection;
    while (!nextLine.empty()) {

        if (nextLine[0] == 'L') {
            nextDirection = false;
        }
        else if (nextLine[0] == 'R') {
            nextDirection = true;
        }
        else {
            std::cout << "wrong direction input at the beginning.." << std::endl;
        }
        directions.push_back(nextDirection);
        nextLine = nextLine.substr(1, nextLine.length());
    }

    std::getline(myfile, nextLine);
    int directionIndex = 0;
    std::vector<InstructionLetters> letterInstructions;

    /* READ INSTRUCTIONS */
    while (std::getline(myfile, nextLine)) {
        std::string from = nextLine.substr(0, 3);
        std::string toLeft = nextLine.substr(7, 3);
        std::string toRight = nextLine.substr(12, 3);
        InstructionLetters nextInstruction(from, toLeft, toRight);
        letterInstructions.push_back(nextInstruction);
    }

    int countSteps = 0;
    std::vector<std::string> startElements;

    /* FILL START ELEMENTS */
    for (InstructionLetters instruction : letterInstructions) {
        if (instruction.from[2] == 'A') {
            startElements.push_back(instruction.from);
        }
    }

    std::vector<int> startNumbers;
    std::vector<int> endNumbers;
    /* CONVERT ELEMENTS TO NUMBERS */
    std::map<std::string, int> lettersToNumbers;
    for (int instructionIndex = 0; instructionIndex < letterInstructions.size(); instructionIndex++) {
        lettersToNumbers[letterInstructions.at(instructionIndex).from] = instructionIndex;
        if (letterInstructions.at(instructionIndex).from[2] == 'A') {
            startNumbers.push_back(instructionIndex);
        }
        if (letterInstructions.at(instructionIndex).from[2] == 'Z') {
            endNumbers.push_back(instructionIndex);
        }
    }

    std::vector<Instruction> instructions;
    std::map<int, int> instructionsLeft;
    std::map<int, int> instructionsRight;
    /* InstructionList as numbers */
    for (int instructionIndex = 0; instructionIndex < letterInstructions.size(); instructionIndex++) {
        InstructionLetters in = letterInstructions.at(instructionIndex);
        Instruction out(lettersToNumbers[in.from], lettersToNumbers[in.toLeft], lettersToNumbers[in.toRight]);
        instructions.push_back(out);
        instructionsLeft[lettersToNumbers[in.from]] = lettersToNumbers[in.toLeft];
        instructionsRight[lettersToNumbers[in.from]] = lettersToNumbers[in.toRight];
    }

    Step steps[6] = {
        Step(startNumbers.at(0), instructionsLeft[startNumbers.at(0)], instructionsRight[startNumbers.at(0)], 0),
        Step(startNumbers.at(1), instructionsLeft[startNumbers.at(1)], instructionsRight[startNumbers.at(1)], 0),
        Step(startNumbers.at(2), instructionsLeft[startNumbers.at(2)], instructionsRight[startNumbers.at(2)], 0),
        Step(startNumbers.at(3), instructionsLeft[startNumbers.at(3)], instructionsRight[startNumbers.at(3)], 0),
        Step(startNumbers.at(4), instructionsLeft[startNumbers.at(4)], instructionsRight[startNumbers.at(4)], 0),
        Step(startNumbers.at(5), instructionsLeft[startNumbers.at(5)], instructionsRight[startNumbers.at(5)], 0)
    };

    bool foundAllZ = false;
    bool foundZ[6] = { false, false, false, false, false, false };
    int stepCounter = 0;
    directionIndex = 0;
    std::vector<int> allPatterns[6];
    std::vector<int> patternOfJ;
    for (int i = 0; i < 6; i++) {
        allPatterns[i].push_back(0);
    }
    while (!foundAllZ) {

        bool b = directions.at(directionIndex);
        for (int track = 0; track < 6; track++) {
            Step next = steps[track];
            if (directions.at(directionIndex)) {
                next.at = steps[track].nextRight;
                next.nextLeft = instructionsLeft[next.at];
                next.nextRight = instructionsRight[next.at];
                next.stepsToGetHere = steps[track].stepsToGetHere + 1;
            }
            else {
                next.at = steps[track].nextLeft;
                next.nextLeft = instructionsLeft[next.at];
                next.nextRight = instructionsRight[next.at];
                next.stepsToGetHere = steps[track].stepsToGetHere + 1;
            }
            steps[track] = next;
        }
        stepCounter++;

        directionIndex++;
        if (directionIndex == directions.size()) {
            directionIndex = 0;
        }

        for (int target : endNumbers) {
            for (int j = 0; j < 6; j++) {
                if (steps[j].at == target) {
                    foundZ[j] = true;
                    allPatterns[j].push_back(stepCounter - allPatterns[j].at(allPatterns[j].size() - 1));
                    std::cout << "pattern of " << j << " : ";
                    for (int i = 0; i < allPatterns[j].size(); i++) {
                        std::cout << allPatterns[j].at(i) << " ";
                    }
                    std::cout << std::endl;
                }
            }
        }

        if (foundZ[0] && foundZ[1] && foundZ[2] && foundZ[3] && foundZ[4] && foundZ[5]) {
            foundAllZ = true;
        }
        else {
            for (int j = 0; j < 6; j++) {
                foundZ[j] = false;
            }
        }
    }
    std::cout << "finished with steps: " << stepCounter << std::endl;
    exit(1);






    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 04" << std::endl;
    std::cout << "Total steps: " << countSteps << std::endl;

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

Step followLeft(Step step, int nextLeft, int nextRight) {
    Step next(step.nextLeft, nextLeft, nextRight, step.stepsToGetHere + 1);
    return next;
}

Step followLRight(Step step, int nextLeft, int nextRight) {
    Step next(step.nextRight, nextLeft, nextRight, step.stepsToGetHere + 1);
    return next;
}