#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>

struct Rule {
    long long inputGenerator;
    long long inputStart;
    long long inputEnd;
    long long destStart;
    long long destEnd;
};

struct NumberBlock {

    NumberBlock(long long s, long long e) : start(s), end(e) {}

    long long start;
    long long end;
};

struct ConvertingMap {
    std::vector<Rule> rules;
};

long long toLongLong(std::string s);
void applyRuleOnBlock(Rule rule, int index, std::vector<NumberBlock>* blocks, int* blockLength);
int binarySearch(const std::vector<Rule>& sortedVector, long long targetValue);

std::vector<NumberBlock> nextBlocks, unconvertedBlocks;

int main()
{
    auto programm_time_start = std::chrono::system_clock::now();

    /* OPEN STREAM */
    std::ifstream myfile;
    myfile.open("ressources/input.txt");
    if (!myfile) {
        std::cerr << "Error: file count not be opened" << std::endl;
        exit(1);
    }

    /* DEFINE VARIBLES*/
    long long destStart, inputStart, inputGenerator;
    long long pos;

    std::vector<long long> seedsIn, seedsOut;
    std::string nextSeedNumberString;
    bool firstMapLine = true;
    bool readRange = false;

    long long nextSeed;
    std::string nextSeedString;
    bool lineFinished = false, odd = true;

    std::vector<ConvertingMap> maps;
    std::vector<NumberBlock> blocks;

    std::vector<long long> firstLineNumbers;

    /* READ AND GENERATE SEEDS (FIRST INPUT LINE) */
    std::string nextLine;
    std::getline(myfile, nextLine);

    std::string restOfLine = nextLine.substr(7, nextLine.length());
    pos = 0;

    while (restOfLine != "") {
        while (restOfLine[pos] == ' ') { pos++; }
        restOfLine = restOfLine.substr(pos, restOfLine.length());
        nextSeedString = restOfLine.substr(0, restOfLine.find(' '));
        nextSeed = toLongLong(nextSeedString);
        firstLineNumbers.push_back(nextSeed);
        restOfLine = restOfLine.substr(nextSeedString.length(), restOfLine.length());
        pos = 0;
    }

    // MAP RULES
    ConvertingMap currentMap;
    bool isFirst = true;
    while (std::getline(myfile, nextLine)) {

        // EMPTY LINE --> SKIP
        if (nextLine.empty()) { continue; }

        // NEW MAP LINE --> NEW MAP + SKIP
        if (!isdigit(nextLine[0])) {
            if (!isFirst) { maps.push_back(currentMap); }
            else { isFirst = false; }

            ConvertingMap newMap;
            currentMap = newMap;
            continue;
        }

        // EACH RULE LINE --> destStart inputStart inputGenerator
        std::string firstNumber = nextLine.substr(0, nextLine.find(' '));
        destStart = toLongLong(firstNumber);
        pos = firstNumber.length() + 1;

        nextLine = nextLine.substr(pos, nextLine.length());
        std::string secondNumber = nextLine.substr(0, nextLine.find(' '));
        inputStart = toLongLong(secondNumber);
        pos = secondNumber.length() + 1;

        nextLine = nextLine.substr(pos, nextLine.length());
        std::string thirdNumber = nextLine.substr(0, nextLine.length());
        inputGenerator = toLongLong(thirdNumber);

        // MAKE NEW RULE AND ADD IT TO THE MAP
        Rule newRule;
        newRule.inputGenerator = inputGenerator;
        newRule.inputStart = inputStart;
        newRule.inputEnd = inputStart + inputGenerator;
        newRule.destStart = destStart;
        newRule.destEnd = destStart + inputGenerator;

        currentMap.rules.push_back(newRule);
    }
    maps.push_back(currentMap);

    for (auto& map : maps) {
        auto& rules = map.rules;
        std::sort(rules.begin(), rules.end(), [](const Rule& a, const Rule& b) {
            return a.inputStart < b.inputStart;
        });
    }

    // FILL BLOCKS
    blocks.clear();
    for (int index = 0; index < firstLineNumbers.size(); index += 2) {

        long long startNumber = firstLineNumbers.at(index);
        long long endNumber = startNumber + firstLineNumbers.at(index + 1);

        NumberBlock startBlock(startNumber, endNumber);

        blocks.push_back(startBlock);
    }

    Rule rule;
    for (int k = 0; k < maps.size(); k++) {
        int blocksLength = blocks.size();
        for (int j = 0; j < blocksLength; j++) {
            auto& rules = maps.at(k).rules;
            int ruleIndex = binarySearch(rules, blocks.at(j).start);

            while (ruleIndex < rules.size() && rules.at(ruleIndex).inputEnd > blocks.at(j).start) {
                applyRuleOnBlock(rule = rules.at(ruleIndex), j, &blocks, &blocksLength);
                ruleIndex++;
            }
        }
    }

    long long smallest = std::numeric_limits<long long>::max();
    for (NumberBlock block : blocks) {
        if (block.start < smallest && block.start < block.end) {
            smallest = block.start;
        }
    }

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 05" << std::endl;
    std::cout << "smallest: " << smallest << std::endl;

    /* TIMER */
    auto programm_time_end = std::chrono::system_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(programm_time_end - programm_time_start);
    std::cout << "Total time needed: " << totalTime.count() << " ms\n";
}

long long toLongLong(std::string s) {

    long long i = 0;
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

void applyRuleOnBlock(Rule rule, int index, std::vector<NumberBlock>* blocks, int* blockLength) {

    long long blockStart = blocks->at(index).start;
    long long blockEnd = blocks->at(index).end;

    long long potentialStart = std::max(rule.inputStart, blockStart);
    long long potentialEnd = std::min(rule.inputEnd, blockEnd);
    
    // outer cases
    bool isOverlapping = potentialEnd > potentialStart;
    if (!isOverlapping) {
        return;
    }

    // new block
    long long cuttedStart = rule.destStart + (potentialStart - rule.inputStart);
    long long cuttedEnd = rule.destStart + (potentialEnd - rule.inputStart);
    NumberBlock cuttedBlock(cuttedStart, cuttedEnd);
    blocks->push_back(cuttedBlock);

    // chunky case
    if (blockStart < rule.inputStart && blockEnd > rule.inputEnd) {
        blocks->at(index).end = rule.inputStart;
        NumberBlock extraBlock(rule.inputEnd, blockEnd);
        if (blocks->size() > *blockLength) {
            blocks->push_back(blocks->at(*blockLength));
            blocks->at(*blockLength) = extraBlock;

        }
        else {
            blocks->push_back(extraBlock);
        }
        *blockLength = *blockLength + 1;
        return;
    }

    // left overlap case
    if (blockStart < rule.inputStart) {
        blocks->at(index).end = rule.inputStart;
        return;
    }

    // right overlap case
    if (blockEnd > rule.inputEnd) {
        blocks->at(index).start = rule.inputEnd;
        return;
    }
    blocks->at(index).start = 0;
    blocks->at(index).end = 0;
}

int binarySearch(const std::vector<Rule>& sortedVector, long long targetValue) {
    int low = 0;
    int high = static_cast<int>(sortedVector.size()) - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (sortedVector[mid].inputStart == targetValue) {
            return mid; // Element found
        }
        else if (sortedVector[mid].inputStart < targetValue) {
            low = mid + 1;
            result = mid; // Update result for next smallest
        }
        else {
            high = mid - 1;
        }
    }

    return result; // Element not found, return the next smallest position
}