#import <string>

struct Submission {
    int id;
    std::string title;
    int primary;
    int secondary; // 0 if it doesn't exist
};

struct Reviewer {
    int id;
    std::string name;
    int primary;
    int secondary;
};

struct Config {
    int minReviews;
    int maxLoad;
    int genAssignments;
    int riskAnalysis;
    std::string outputFileName = "output.txt";
};