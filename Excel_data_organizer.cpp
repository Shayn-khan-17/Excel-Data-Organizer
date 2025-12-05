#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Pair {
    double first;  // First value (1st column)
    double second; // Second value (2nd column)
};

int main() {
    std::vector<std::vector<Pair>> cycles; // Store up to 12 cycles
    std::vector<Pair> currentCycle;        // Current cycle being processed
    bool inPositiveCycle = false;          // Tracks if currently in a positive cycle
    const int maxCycles = 12;              // Maximum of 12 cycles
    double first, second;

    std::cout << "Enter pairs of values (first second). Enter '-999 -999' to stop input:\n";

    // Continuously read pairs of values
    while (true) {
        std::cin >> first >> second;
        if (first == -999 && second == -999) {
            break; // Stop input when both values are -999
        }

        if (second > 0) {
            // Second value is positive
            if (!inPositiveCycle) {
                // Transition from negative to positive, finalize the current cycle
                if (!currentCycle.empty()) {
                    cycles.push_back(currentCycle);
                    currentCycle.clear(); // Reset for new cycle

                    // Stop if we have filled 12 cycles
                    if (cycles.size() >= maxCycles) {
                        std::cout << "Maximum cycles reached. Excess data will be ignored.\n";
                        break;
                    }
                }
                inPositiveCycle = true;
            }
            currentCycle.push_back({first, second});
        } else if (second < 0) {
            // Second value is negative
            currentCycle.push_back({first, second});
            inPositiveCycle = false; // Mark as out of positive cycle
        }
    }

    // Add the last cycle if not empty
    if (!currentCycle.empty() && cycles.size() < maxCycles) {
        cycles.push_back(currentCycle);
    }

    // Create a CSV file to write the output
    std::ofstream outputFile("Cycles data End bearing.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not create output file!\n";
        return 1;
    }

    // Write headers for 24 columns (12 cycles with two sub-columns each)
    for (int i = 0; i < maxCycles; ++i) {
        outputFile << "Cycle " << i + 1 << " First,"
                   << "Cycle " << i + 1 << " Second,";
    }
    outputFile << "\n";

    // Find the maximum number of rows across all cycles
    size_t maxRows = 0;
    for (const auto& cycle : cycles) {
        maxRows = std::max(maxRows, cycle.size());
    }

    // Write the data row by row, padding with empty cells when necessary
    for (size_t row = 0; row < maxRows; ++row) {
        for (size_t cycleIdx = 0; cycleIdx < maxCycles; ++cycleIdx) {
            if (cycleIdx < cycles.size() && row < cycles[cycleIdx].size()) {
                // Write values from the current cycle
                outputFile << cycles[cycleIdx][row].first << ","
                           << cycles[cycleIdx][row].second << ",";
            } else {
                // If data is missing (e.g., shorter cycle), write empty cells
                outputFile << ",,";
            }
        }
        outputFile << "\n";
    }

    outputFile.close();
    std::cout << "Data successfully written to Cycles data End bearing.csv\n";

    return 0;
}
