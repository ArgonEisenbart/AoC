#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

int main()
{
    auto time_start = std::chrono::system_clock::now();

    /* OPEN STREAM */
    std::ifstream myfile;
    myfile.open("ressources/input2.txt");
    if (!myfile) {
        std::cerr << "Error: file count not be opened" << std::endl;
        exit(1);
    }

    /* DEFINE VARIBLES*/
    std::size_t sum = 0;
    std::size_t multiplier = 10;
    std::vector<std::vector<char>> originalImage;
    std::vector<std::vector<char>> expandedImage;

    /* VAR 2 */
    std::vector<std::vector<std::size_t>> distanceMap;

    /* READ IMAGE */
    std::string nextLine;
    while (std::getline(myfile, nextLine)) {
        std::vector<char> nextRow;
        for (std::size_t i = 0; i < nextLine.length(); i++) {
            nextRow.push_back(nextLine[i]);
        }
        originalImage.push_back(nextRow);
    }

    /* EXPAND IMAGE */
    /*
    bool emptyLine = true;
    for (int i = 0; i < originalImage.size(); i++) {
        for (int j = 0; j < originalImage.at(0).size(); j++) {
            if (originalImage.at(i).at(j) == '#') {
                emptyLine = false;
            }
        }
        if (emptyLine) {
            expandedImage.push_back(originalImage.at(i));
        }
        expandedImage.push_back(originalImage.at(i));
        emptyLine = true;
    }

    bool emptyColoum = true;
    int rowLength = expandedImage.at(0).size();
    int extra = 0;
    for (int j = 0; j < rowLength; j++) {
        for (int i = 0; i < expandedImage.size(); i++) {
            if (expandedImage.at(i).at(j) == '#') {
                emptyColoum = false;
            }
        }
        if (emptyColoum) {
            std::vector<char> newLine;
            std::vector<int> newDistanceLine;
            for (int k = 0; k < expandedImage.size(); k++) {
                for (int l = 0; l < rowLength + extra; l++) {
                    if (l == j) {
                        newLine.push_back(expandedImage.at(k).at(l));
                    }
                    newLine.push_back(expandedImage.at(k).at(l));
                }
                expandedImage.at(k) = newLine;
                newLine.clear();
            }
            j++;
            extra++;
        }
        emptyColoum = true;
    }
    */
    /*
    std::cout << "before expanding: " << std::endl;
    for (int i = 0; i < distanceMap.size(); i++) {
        for (int j = 0; j < distanceMap.at(0).size(); j++) {
            std::cout << distanceMap.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
    */

    /* EXPAND IMAGE */
    bool emptyLine = true;
    std::vector<std::size_t> distanceRow;
    for (std::size_t i = 0; i < originalImage.size(); i++) {
        for (std::size_t j = 0; j < originalImage.at(0).size(); j++) {
            if (originalImage.at(i).at(j) == '#') {
                emptyLine = false;
            }
        }
        if (emptyLine) {
            for (std::size_t d = 0; d < originalImage.at(0).size(); d++) {
                distanceRow.push_back(multiplier);
            }
            distanceMap.push_back(distanceRow);
            distanceRow.clear();
        }
        else {
            for (std::size_t d = 0; d < originalImage.at(0).size(); d++) {
                distanceRow.push_back(1);
            }
            distanceMap.push_back(distanceRow);
            distanceRow.clear();
        }
        emptyLine = true;
    }

    bool emptyColoum = true;
    std::size_t rowLength = originalImage.at(0).size();
    for (std::size_t j = 0; j < rowLength; j++) {
        for (std::size_t i = 0; i < originalImage.size(); i++) {
            if (originalImage.at(i).at(j) == '#') {
                emptyColoum = false;
            }
        }
        if (emptyColoum) {
            std::vector<std::size_t> newDistanceLine;
            for (std::size_t k = 0; k < originalImage.size(); k++) {
                for (std::size_t l = 0; l < rowLength; l++) {
                    if (l == j) {
                        newDistanceLine.push_back(multiplier);
                    }
                    else {
                        newDistanceLine.push_back(distanceMap.at(k).at(l));
                    }
                }
                distanceMap.at(k) = newDistanceLine;
                newDistanceLine.clear();
            }
            j++;
        }
        emptyColoum = true;
    }

    /* NUMERATE GALAXIES */
    std::vector<std::size_t> galaxyPositionsX;
    std::vector<std::size_t> galaxyPositionsY;

    for (std::size_t y = 0; y < originalImage.size(); y++) {
        for (int x = 0; x < originalImage.at(0).size(); x++) {
            if (originalImage.at(y).at(x) == '#') {
                galaxyPositionsX.push_back(x);
                galaxyPositionsY.push_back(y);
            }
        }
    }

    /* ALL COMBINATIONS */
    std::vector<std::size_t> distances;
    std::size_t singleDistance = 0;
    for (std::size_t galaxyIndexA = 0; galaxyIndexA < galaxyPositionsX.size(); galaxyIndexA++) {
        for (std::size_t galaxyIndexB = galaxyIndexA + 1; galaxyIndexB < galaxyPositionsX.size(); galaxyIndexB++) {

            // A.X <= B.X
            if (galaxyPositionsX.at(galaxyIndexA) < galaxyPositionsX.at(galaxyIndexB) + 1) {
                // A.Y <= B.Y
                // A ist oben links von B
                if (galaxyPositionsY.at(galaxyIndexA) < galaxyPositionsY.at(galaxyIndexB) + 1) {
                    for (std::size_t pathingX = galaxyPositionsX.at(galaxyIndexA) + 1; pathingX < galaxyPositionsX.at(galaxyIndexB) + 1; pathingX++) {
                        singleDistance += distanceMap.at(galaxyPositionsY.at(galaxyIndexA)).at(pathingX);
                    }
                    for (std::size_t pathingY = galaxyPositionsY.at(galaxyIndexA) + 1; pathingY < galaxyPositionsY.at(galaxyIndexB) + 1; pathingY++) {
                        singleDistance += distanceMap.at(pathingY).at(galaxyPositionsX.at(galaxyIndexA));
                    }
                }
                // A.Y > B.Y
                // A ist unten links von B
                else /*(galaxyPositionsY.at(galaxyIndexA) <= galaxyPositionsY.at(galaxyIndexB))*/ {
                    for (std::size_t pathingX = galaxyPositionsX.at(galaxyIndexA) + 1; pathingX < galaxyPositionsX.at(galaxyIndexB) + 1; pathingX++) {
                        singleDistance += distanceMap.at(galaxyPositionsY.at(galaxyIndexA)).at(pathingX);
                    }
                    for (std::size_t pathingY = galaxyPositionsY.at(galaxyIndexA) - 1; pathingY > galaxyPositionsY.at(galaxyIndexB) - 1; pathingY--) {
                        singleDistance += distanceMap.at(pathingY).at(galaxyPositionsX.at(galaxyIndexA));
                    }
                }
            }
            // A.X > B.X
            else {
                // A.Y <= B.Y
                // A ist oben rechts von B
                if (galaxyPositionsY.at(galaxyIndexA) <= galaxyPositionsY.at(galaxyIndexB)) {
                    for (std::size_t pathingX = galaxyPositionsX.at(galaxyIndexA) - 1; (pathingX > galaxyPositionsX.at(galaxyIndexB) - 1); pathingX--) {
                        singleDistance += distanceMap.at(galaxyPositionsY.at(galaxyIndexA)).at(pathingX);
                    }
                    for (std::size_t pathingY = galaxyPositionsY.at(galaxyIndexA) + 1; pathingY < galaxyPositionsY.at(galaxyIndexB) + 1; pathingY++) {
                        singleDistance += distanceMap.at(pathingY).at(galaxyPositionsX.at(galaxyIndexA));
                    }
                }
                // A.Y > B.Y
                // A ist unten rechts von B
                else /*(galaxyPositionsY.at(galaxyIndexA) <= galaxyPositionsY.at(galaxyIndexB))*/ {
                    for (std::size_t pathingX = galaxyPositionsX.at(galaxyIndexA) - 1; pathingX > galaxyPositionsX.at(galaxyIndexB) - 1; pathingX--) {
                        singleDistance += distanceMap.at(galaxyPositionsY.at(galaxyIndexA)).at(pathingX);
                    }
                    for (std::size_t pathingY = galaxyPositionsY.at(galaxyIndexA) - 1; pathingY > galaxyPositionsY.at(galaxyIndexB) - 1; pathingY--) {
                        singleDistance += distanceMap.at(pathingY).at(galaxyPositionsX.at(galaxyIndexA));
                    }
                }
            }
            distances.push_back(singleDistance);
            singleDistance = 0;
        }
    }

    /* SUM DISTANCES */
    for (std::size_t i = 0; i < distances.size(); i++) {
        sum += distances.at(i);
    }

    /* OUTPUT RESULT */
    std::cout << "Advent of Code! - Day 11" << std::endl;
    std::cout << "Sum of all shortest paths: " << sum << std::endl;

    /* TIMER */
    auto time_end = std::chrono::system_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Total time needed: " << totalTime.count() << " ms\n";
}