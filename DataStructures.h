/**
* @file DataStructures.h
 * @brief Useful Data Structures.
 */
#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <string>

/**
 * @brief Represents a scientific article submission.
 */
struct Submission {
    int id;
    std::string title;
    std::string authors;
    std::string email;
    int primary;
    int secondary;
};

/**
 * @brief Represents a reviewer and their domain expertise(s).
 */
struct Reviewer {
    int id;
    std::string name;
    std::string email;
    int primary;
    int secondary;
};

/**
 * @brief Global configuration parameters for the algorithm.
 * @note Defines the capacity constraints for the graph.
 */
struct Parameters {
    int minReviewsSub;
    int minReviewsRev;
    int primaryExpertise;
    int secondaryExpertise;
    int primaryDomain;
    int secondaryDomain;
};

/**
 * @brief Configuration parameters to control the algorithm's execution and output.
 */
struct Control
{
    int genAssignments;
    int riskAnalysis;
    std::string outputFileName = "output.txt";
};
#endif //DATASTRUCTURES_H