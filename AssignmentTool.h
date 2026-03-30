//
// Created by pedrodomingos on 2026-03-21.
//
/**
 * @file AssignmentTool.h
 * @brief Core logic for graph construction, flow calculation, and risk analysis.
 */
#ifndef PROJETO1_ASSIGNMENTTOOL_H
#define PROJETO1_ASSIGNMENTTOOL_H

#include "DataStructures.h"
#include "Graph.h"
#include "Parser.h"
#include <vector>
#include <string>
#include <queue>

using namespace std;

/**
 * @brief Aggregates all data parsed from the dataset.
 */
struct graph_info {
    vector<Submission> submissions;
    vector<Reviewer> reviewers;
    Parameters parameters;
    Control control;
};

/**
 * @brief Orchestrates the parsing process to fill a graph_info struct.
 * @param filename Path to the CSV file.
 * @return Filled graph_info structure.
 */
graph_info populateInfo(const string& filename);

/**
 * @brief Creates the graph structure with Source and Target nodes.
 * @param info Uses the graph_info struct to build the vertices and edges.
 * @return Pointer to the initialized graph.
 */
Graph<string>* createGraph(graph_info info);

/**
 * @brief Generates valid edges between submissions and reviewers based on constraints.
 * @param g The graph being built.
 * @param subs Vector of submissions.
 * @param revs Vector of reviewers.
 * @param ctrl Control struct to determine the assignment mode.
 */
void runGenerateAssignments(Graph<string>* g, vector<Submission> subs, vector<Reviewer> revs, Control ctrl);

/**
 * @brief Identifies critical reviewers whose absence prevents full assignment.
 * @return Vector of IDs of critical reviewers.
 */
vector<int> runRiskAnalysis(graph_info info);

/**
 * @brief Implementation of the Edmonds-Karp algorithm to find Max Flow.
 * @tparam T Vertex type.
 * @param g Pointer to the graph.
 * @param source Identifier of the source vertex.
 * @param target Identifier of the target vertex.
 * @note Complexity: O(V*E^2).
 */
template <class T>
void runMaxFlowEdmondsKarp(Graph<T> *g, string source, string target);

/**
 * @brief Checks if all submissions reached their minimum review requirement.
 * @param g Pointer to the graph.
 * @param params Algorithm parameters.
 * @return True if assignment is valid, False otherwise.
 */
bool isAssignmentValid(const Graph<string>* g,Parameters params);



#endif //PROJETO1_ASSIGNMENTTOOL_H