//
// Created by pedrodomingos on 2026-03-21.
//
#include "Graph.h"
#include "DataStructures.h"
#include <vector>
#include <iostream>
#include <string>
#include "Parser.h"
#include "AssignmentTool.h"
using namespace std;

void displayAssignmentResults(Graph<string>* graph, const graph_info& info) {
    cout << "\n=== RESULTS DISPLAY ===" << endl;

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

                        cout << "Submission '" << sub_vertex->getInfo()
                             << "' assigned to Reviewer '" << rev_vertex->getInfo() << "'" << endl;

                        num_reviews++;
                        totalAssignments++;
                    }
                }

                if (num_reviews < info.parameters.minReviewsSub) {
                    cout << "  -> [WARNING] Submission '" << sub_vertex->getInfo()
                         << "' needs " << (info.parameters.minReviewsSub - num_reviews)
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
                        cout << "Source -> " << sub_vertex->getInfo()
                             << " -> " << rev_vertex->getInfo() << " -> Target" << endl;
                    }
                }
            }
        }
    }
    cout << "-----------------------" << endl;
    cout << "Total Assignments Made: " << totalAssignments << endl;
}


void displayRiskAnalysisResults(const vector<int>& risky_reviewers) {
    cout << "At-risk Reviewers (IDs): ";
    if (risky_reviewers.empty()) {
        cout << "None. The assignment is always possible!" << endl;
    } else {
        for (int id : risky_reviewers) {
            cout << id << " ";
        }
        cout << endl;
    }
}


void runMenu() {
    int choice = 0;
    string inputFile;

    cout << "Enter the input file: ";
    cin >> inputFile;

    graph_info info = populateInfo(inputFile);

    if (info.submissions.empty() || info.reviewers.empty()) {
        cerr << "Parsing Error" << endl;
        return;
    }

    while (true) {
        cout << "\n--- Scientific Conference Organization Tool ---" << endl;
        cout << "1. Run Assignment" << endl;
        cout << "2. Run Risk Analysis" << endl;
        cout << "3. Exit" << endl;
        cout << "Select an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore('\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "\nRunning Assignment..." << endl;
                Graph<string>* graph = createGraph(info);
                if (graph != nullptr) {
                    cout << "Processing successfully completed" << endl;



                    // Output RiskAnalysis

                    delete graph;
                }
                break;
            }
            case 2: {
                cout << "\nRunning Risk Analysis..." << endl;
                vector<int> risky_reviewers = runRiskAnalysis(info);

                // Output RiskAnalysis
            }
            case 3:
                cout << "Exiting the program. Good work!" << endl;
                return;
            default:
                cout << "Invalid option. Choose between 1 and 3." << endl;
        }
    }
}



void runBatchMode(const string& inputFile, const string& outputFile) {

    cout << "Input: " << inputFile << " | Output: " << outputFile << endl;

    graph_info info = populateInfo(inputFile);
    if (info.submissions.empty() || info.reviewers.empty()) {
        cerr << "Parsing Error" << endl;
        return;
    }

    Graph<string>* graph = createGraph(info);
    if (graph != nullptr) {
        cout << "Processing successfully completed" << endl;

        if (info.control.riskAnalysis > 0) {
            vector<int> risky_reviewers = runRiskAnalysis(info);
            // Output RiskAnalysis
        }

        // put on output file

        delete graph;
    }
}


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