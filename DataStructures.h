/**
* @file DataStructures.h
 * @brief Useful Data Structures.
 */
#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <string>

/**
 * @brief Represents a scientific article submission.
 * @note Complexity: O(N) where N is the length of the title string.
 */
struct Submission {
    int id;
    std::string title;
    int primary;
    int secondary;
};

/**
 * @brief Represents a reviewer and their domain expertise(s).
 * @note Complexity: O(N) where N is the length of the name string.
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
struct Parameters {
    int minReviewsSub;
    int minReviewsRev;
    int primaryExpertise;
    int secondaryExpertise;
    int primaryDomain;
    int secondaryDomain;
};

struct Control
{
    int genAssignments;
    int riskAnalysis;
    std::string outputFileName = "output.txt";
};
#endif //DATASTRUCTURES_H