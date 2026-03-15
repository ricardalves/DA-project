#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <string>
/**
* @file DataStructures.h
 * @brief Useful Data Structures.
 */

/**
 * @brief Represents a scientific article submission.
 * @note Space Complexity: O(N) where N is the length of the title string.
 */
struct Submission {
    int id;
    std::string title;
    int primary;
    int secondary; // 0 if it doesn't exist
};

/**
 * @brief Represents a reviewer and their domain expertise(s).
 * @note Space Complexity: O(N) where N is the length of the name string.
 */
struct Reviewer {
    int id;
    std::string name;
    int primary;
    int secondary;
};

/**
 * @brief Global configuration parameters for the assignment algorithm.
 * @note Defines the capacity constraints for the Network Flow graph.
 */
struct Config {
    int minReviews;
    int maxLoad;
    int genAssignments;
    int riskAnalysis;
    std::string outputFileName = "output.txt";
};
#endif //DATASTRUCTURES_H