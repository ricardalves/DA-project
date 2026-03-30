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