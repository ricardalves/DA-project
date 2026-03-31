/**
* @file main.cpp
 * @brief Entry point and user interface for the program
 */
#include "Output.h"
#include "Graph.h"
#include "DataStructures.h"
#include <vector>
#include <iostream>
#include <string>
#include "Parser.h"
#include "AssignmentTool.h"
using namespace std;

/**
 * @brief Displays the matching results of the assignment algorithm.
 */
void displayAssignmentResults(Graph<string>* graph, const graph_info& info) {
    cout << "\n\n=== RESULTS DISPLAY ===" << endl;

    Vertex<string>* source = graph->findVertex("Source");
    int totalAssignments = 0;

    if (source != nullptr) {
        cout << "--- Assigned Reviews ---" << endl;

        for (auto sub_edge : source->getAdj()) {
            if (sub_edge->getFlow() > 0) {
                Vertex<string>* sub_vertex = sub_edge->getDest();
                int num_reviews = 0;

                for (auto rev_edge : sub_vertex->getAdj()) {
                    if (rev_edge->getFlow() > 0 && rev_edge->getDest()->getInfo() != "Source") {
                        Vertex<string>* rev_vertex = rev_edge->getDest();

                        cout << "Submission " << sub_vertex->getNum()
                             << " assigned to Reviewer " << rev_vertex->getNum() << endl;

                        num_reviews++;
                        totalAssignments++;
                    }
                }

                if (num_reviews < info.parameters.minReviewsSub) {
                    cout << "  Submission " << sub_vertex->getNum()
                         << " needs " << (info.parameters.minReviewsSub - num_reviews)
                         << " more review(s)!" << endl;
                }
            }
        }

        cout << "\n--- Full Sequence Paths (Origin -> Destination) ---" << endl;

        for (auto sub_edge : source->getAdj()) {
            if (sub_edge->getFlow() > 0) {
                Vertex<string>* sub_vertex = sub_edge->getDest();
                for (auto rev_edge : sub_vertex->getAdj()) {
                    if (rev_edge->getFlow() > 0 && rev_edge->getDest()->getInfo() != "Source") {
                        Vertex<string>* rev_vertex = rev_edge->getDest();
                        cout << "Source -> " << sub_vertex->getNum()
                             << " -> " << rev_vertex->getNum() << " -> Target" << endl;
                    }
                }
            }
        }
    }
    cout << "-----------------------" << endl;
    cout << "Total Assignments Made: " << totalAssignments << endl;
    cout << "\n";
}

/**
 * @brief Displays the IDs of the reviewers whose absence makes the assignment impossible.
 */
void displayRiskAnalysisResults(const vector<int>& risky_reviewers) {
    cout << "\n\n=== RISK ANALYSIS RESULTS ===" << endl;

    if (risky_reviewers.empty()) {
        cout << "No at-risk reviewers detected!" << endl;
        cout << "           The assignment is always possible." << endl;
    } else {
        cout << "Found " << risky_reviewers.size() << " at-risk reviewer(s)." << endl;
        cout << "IDs: ";

        for (size_t i = 0; i < risky_reviewers.size(); ++i) {
            cout << risky_reviewers[i];
            if (i < risky_reviewers.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    cout << "\n";
}

/**
 * @brief Executes the interactive command-line menu for the user.
 */
void runMenu() {
    int choice = 0;
    string inputFile;

    cout << "Enter the input file: ";
    cin >> inputFile;

    graph_info info = populateInfo(inputFile);

    if (info.submissions.empty() || info.reviewers.empty()) {
        return;
    }

    while (true) {
        cout << "\n===============================================" << endl;
        cout << "   Scientific Conference Organization Tool" << endl;
        cout << "===============================================" << endl;
        cout << " Current Dataset: " << inputFile << endl;
        cout << "-----------------------------------------------" << endl;
        cout << " 1. Load a Different Dataset" << endl;
        cout << " 2. View Current Dataset Info" << endl;
        cout << " 3. Run Assignment" << endl;
        cout << " 4. Run Risk Analysis" << endl;
        cout << " 5. Exit" << endl;
        cout << "===============================================" << endl;
        cout << "Select an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000,'\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                string new_inputFile;
                cout << "\nEnter the new dataset filename: ";
                cin >> new_inputFile;
                graph_info new_info = populateInfo(new_inputFile);
                if (new_info.submissions.empty() || new_info.reviewers.empty()) {
                    cout << "Keeping the previous dataset." << endl;
                } else {
                    inputFile = new_inputFile;
                    info = new_info;
                    cout << "Dataset successfully updated!" << endl;
                }
                break;
            }
            case 2: {
                cout << "\n\n === Dataset Information === " << endl;
                cout << "Total Submissions: " << info.submissions.size() << endl;
                cout << "Total Reviewers: " << info.reviewers.size() << endl;
                cout << "Min Reviews per Submission: " << info.parameters.minReviewsSub << endl;
                cout << "Max Reviews per Reviewer: " << info.parameters.maxReviewsRev << endl;
                cout << "Control - Generate Assignments: " << info.control.genAssignments << endl;
                cout << "Control - Risk Analysis Mode: " << info.control.riskAnalysis << "\n" << endl;
                break;
            }
            case 3: {
                Graph<string>* graph = createGraph(info);
                if (graph != nullptr) {

                    displayAssignmentResults(graph,info);

                    delete graph;
                }
                break;
            }
            case 4: {

                if (info.control.riskAnalysis == 0) {
                    cout << "Risk Analysis is disabled in this dataset" << endl;
                }
                else {
                    vector<int> risky_reviewers = runRiskAnalysis(info);
                    displayRiskAnalysisResults(risky_reviewers);
                }
                break;
            }
            case 5:
                return;
            default:
                cout << "Invalid option. Choose between 1 and 5." << endl;
        }
    }
}

/**
 * @brief Executes the assignment algorithm in batch mode, generating the output file directly.
 */
void runBatchMode(const string& inputFile, const string& outputFile) {

    graph_info info = populateInfo(inputFile);
    if (info.submissions.empty() || info.reviewers.empty()) {
        return;
    }

    info.control.outputFileName = outputFile;

    Graph<string>* graph = createGraph(info);
    if (graph != nullptr) {

        vector<int> risky_reviewers;
        if (info.control.riskAnalysis > 0) {
            risky_reviewers = runRiskAnalysis(info);
        }

        generateOutput(graph, info, risky_reviewers);

        delete graph;
    }
}

/**
 * @brief Main function. Handles command-line arguments to switch between batch and menu modes.
 */
int main(int argc, char* argv[]) {

    if (argc == 4 && string(argv[1]) == "-b") {
        string inputFile = argv[2];
        string outputFile = argv[3];
        runBatchMode(inputFile, outputFile);
    }

    else {
        runMenu();
    }

    return 0;
}