//
// Created by pedrodomingos on 2026-03-21.
//

#ifndef PROJETO1_OUTPUT_H
#define PROJETO1_OUTPUT_H

#include "Graph.h"
#include "DataStructures.h"
#include "Parser.h"
#include "AssignmentTool.h"

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Generates the entire output (Matches, Missing Reviews and Risk analysis).
 * @param g The processed graph with the flow already calculated.
 * @param info Original info from the starting dataset (to get the domains).
 * @param risk Vector with the ID from the critical reviewers.
 */
void generateOutput(const Graph<string>* g, const graph_info& info, const vector<int>& risk);

#endif //PROJETO1_OUTPUT_H